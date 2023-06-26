#pragma once
#include <array>
#include <bitset>
#include <cstdint>
#include <memory>
#include <queue>
#include <vector>
#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xnpy.hpp>
#include <xtensor/xview.hpp>

#include <Engine/Runtime/Utils/Spare.h>
#include <Engine/Runtime/Utils/hilbert.h>

namespace Engine
{
    template<typename T>
    using Ref = std::shared_ptr<T>;

    class OctreeNode
    {
    public:
        enum OctreeNodeType
        {
            None    = 0,
            Branche = 1,
            Leaf    = 2
        };

    public:
        OctreeNode(uint32_t       x,
                   uint32_t       y,
                   uint32_t       z,
                   uint32_t       r,
                   uint32_t       g,
                   uint32_t       b,
                   uint32_t       level,
                   uint32_t       octant,
                   OctreeNodeType type) :
            m_x(x),
            m_y(y), m_z(z), m_r(r), m_g(g), m_b(b), m_Level(level), m_Octant(octant), m_Type(type),
            m_Occupancy("00000000")
        {}

        uint32_t GetX() { return m_x; }
        uint32_t GetY() { return m_y; }
        uint32_t GetZ() { return m_z; }
        uint32_t GetR() { return m_r; }
        uint32_t GetG() { return m_g; }
        uint32_t GetB() { return m_b; }
        uint32_t GetLevel() { return m_Level; }
        uint32_t GetOctant() { return m_Octant; }

        void            SetParent(const Ref<OctreeNode>& parent) { m_Parent = parent; }
        void            SetNextNeighbor(const Ref<OctreeNode>& nex) { m_NextNeighbor = nex; }
        void            SetPrevNeighbor(const Ref<OctreeNode>& pre) { m_PrevNeighbor = pre; }
        Ref<OctreeNode> GetParent() { return m_Parent; }
        Ref<OctreeNode> GetNextNeighbor() { return m_NextNeighbor; }
        Ref<OctreeNode> GetPrevNeighbor() { return m_PrevNeighbor; }

        void SetChildren(const Ref<OctreeNode>& children, const uint32_t& idx)
        {
            m_Childrens[idx] = children;
            m_Occupancy.set(idx, 1);
        }
        Ref<OctreeNode> GetChildren(const uint32_t& idx) { return m_Childrens[idx]; }

    private:
        uint32_t m_x, m_y, m_z;
        uint32_t m_r, m_g, m_b;
        uint32_t m_Level;
        uint32_t m_Octant;

        OctreeNodeType m_Type;

        Ref<OctreeNode> m_Parent;
        Ref<OctreeNode> m_NextNeighbor;
        Ref<OctreeNode> m_PrevNeighbor;

        std::bitset<8>                 m_Occupancy;
        std::array<Ref<OctreeNode>, 8> m_Childrens;
    };

    class Octree
    {
    public:
        Octree(const uint32_t maxLevel);
        Octree(const std::vector<std::array<int, 3>>&      coords,
               const std::vector<std::array<uint32_t, 3>>& feats,
               const uint32_t                              maxLevel);

        const std::vector<Ref<OctreeNode>> GetLevelNodes(const uint32_t level) { return m_LevelNodes[level]; }

        uint32_t GetMaxLevel() { return m_MaxLevel; }
        uint32_t GetNumNodes() { return m_NumNodes; }
        uint32_t GetNumLeafs() { return m_NumLeafs; }

        uint32_t GetLevelNumNodes(const uint32_t& level) { return (uint32_t)m_LevelNodes[level].size(); }

    private:
        void insert(uint32_t x, uint32_t y, uint32_t z, uint32_t r, uint32_t g, uint32_t b);
        void Update();

    private:
        uint32_t        m_MaxLevel;
        uint32_t        m_MaxBound;
        Ref<OctreeNode> m_Root;

        uint32_t m_NumNodes = 0;
        uint32_t m_NumLeafs = 0;

        std::vector<Ref<OctreeNode>>              m_LevelHead;
        std::vector<std::vector<Ref<OctreeNode>>> m_LevelNodes;
    };

    template<class T>
    class HashOctree
    {
        using OctreeLayer = RootBlock3D<T, HashBlock3D<PointerBlock3D<2, DenseBlock3D<1, LeafData<T>>>>>;

    public:
        HashOctree(const std::vector<std::array<int, 3>>& coords,
                   const std::vector<T>&                  feats,
                   const uint32_t                         maxLevel) :
            m_Layers(maxLevel + 1),
            m_MaxLevel(maxLevel), m_Scales(maxLevel + 1)
        {
            for (int l = 0; l <= maxLevel; ++l)
            {
                m_Scales[l] = 1 << (m_MaxLevel - l);
                for (int idx = 0; idx < coords.size(); ++idx)
                {
                    int rshift = (m_MaxLevel - l);
                    int x      = coords[idx][0] >> rshift;
                    int y      = coords[idx][1] >> rshift;
                    int z      = coords[idx][2] >> rshift;
                    m_Layers[l].write(x, y, z, feats[idx]);
                }

                /*
                std::vector<std::array<int, 3>> LevelNode;
                m_Layers[l].foreach ([l, &LevelNode](int x, int y, int z, T& val) {
                    LevelNode.push_back({x, y, z});
                });
                m_LevelNodes.push_back(LevelNode);
                */
            }

            m_LevelNodes = std::vector<std::vector<std::array<int, 3>>>(m_MaxLevel + 1);

            // Oct sort
            std::queue<std::array<int, 4>> que;
            que.push({0, 0, 0, 0});
            while (!que.empty())
            {
                std::array<int, 4> pos = que.front();
                que.pop();
                int x        = pos[0];
                int y        = pos[1];
                int z        = pos[2];
                int nowLevel = pos[3];

                m_LevelNodes[nowLevel].push_back({pos[0], pos[1], pos[2]});

                for (int bbz = 0; bbz <= 1; ++bbz)
                {
                    for (int bby = 0; bby <= 1; ++bby)
                    {
                        for (int bbx = 0; bbx <= 1; ++bbx)
                        {
                            std::array<int, 3> occ_coord = {
                                (x << 1) | bbx, //
                                (y << 1) | bby, //
                                (z << 1) | bbz  //
                            };

                            if (nowLevel + 1 <= m_MaxLevel)
                            {
                                bool occ_coord_has =
                                    m_Layers[nowLevel + 1].has(occ_coord[0], occ_coord[1], occ_coord[2]);

                                if (occ_coord_has == true)
                                {
                                    que.push({occ_coord[0], occ_coord[1], occ_coord[2], nowLevel + 1});
                                }
                            }
                        }
                    }
                }
            }

            /*
            for (int i = 0; i <= m_MaxLevel; ++i) {
                auto& level = m_LevelNodes[i];
                std::sort(
                    level.begin(), level.end(),
                    [&i](const std::array<int, 3>& po1,
                         const std::array<int, 3>& po2) {
                        bitmask_t p1[3] = {(bitmask_t)po1[0], (bitmask_t)po1[1],
                                           (bitmask_t)po1[2]};
                        bitmask_t p2[3] = {(bitmask_t)po2[0], (bitmask_t)po2[1],
                                           (bitmask_t)po2[2]};
                        bitmask_t h1 = hilbert_c2i(3, 12, p1);
                        bitmask_t h2 = hilbert_c2i(3, 12, p2);
                        return h1 > h2;
                    }

                );
            }
            */

            return;
        }

        const std::tuple<std::vector<std::array<int, 3>>, std::vector<T>> GetLevelNodes(const uint32_t level)
        {
            std::vector<std::array<int, 3>> coords = m_LevelNodes[level];
            std::vector<T>                  feats;
            for (const auto& co : coords)
            {
                feats.push_back(m_Layers[level].read(co[0], co[1], co[2]));
            }

            return {coords, feats};
        }

        uint32_t GetMaxLevel() { return m_MaxLevel; }
        uint32_t GetNumNodes()
        {
            uint32_t count = 0;
            for (const auto& lever : m_LevelNodes)
            {
                count += (uint32_t)lever.size();
            }
            return count;
        }
        uint32_t GetNumLeafs() { return (uint32_t)m_LevelNodes[m_MaxLevel].size(); }

        uint32_t GetLevelNumNodes(const uint32_t level) { return (uint32_t)m_LevelNodes[level].size(); }

        uint32_t GetScale(const uint32_t level) { return m_Scales[level]; }

        xt::xarray<int> GetContexBatch(const int numNeighbor, const int numAncLevel)
        {
            int numBatches = 0;
            for (int i = 0; i <= m_MaxLevel - 1; ++i)
            {
                numBatches += m_LevelNodes[i].size();
            }

            xt::xarray<int> Batch = xt::zeros<int>({numBatches, numNeighbor, numAncLevel + 1, 3});

            int idxBatch = 0;
            for (int level = 0; level <= m_MaxLevel - 1; ++level)
            {
                for (int idxNode = 0; idxNode < m_LevelNodes[level].size(); ++idxNode)
                {
                    xt::view(Batch, idxBatch, xt::all(), xt::all(), xt::all()) =
                        GetContex(level, idxNode, numNeighbor, numAncLevel);
                    // std::cout << idxBatch << " " << numBatches << std::endl;
                    ++idxBatch;
                }
            }

            return Batch;
        }

        xt::xarray<int> GetContex(const int level, const int idx, const int numNeighbor, const int numAncLevel)
        {
            xt::xarray<int> hiks = xt::zeros<int>({numNeighbor, numAncLevel + 1, 3});
            if (level >= m_MaxLevel)
            {
                return hiks;
            }

            std::vector<std::array<int, 3>> neighbor_coords;
            for (int i = idx; i >= idx - (numNeighbor - 1) && i >= 0; --i)
            {
                neighbor_coords.push_back(m_LevelNodes[level][i]);
            }

            for (int i = 0; i < neighbor_coords.size(); ++i)
            {
                auto nowNode = neighbor_coords[i];
                for (int sLevel = 0; sLevel <= numAncLevel && level - sLevel >= 0; ++sLevel)
                {
                    int                rShift     = sLevel;
                    int                nowLevel   = level - sLevel;
                    std::array<int, 3> anc_coords = {nowNode[0] >> rShift, nowNode[1] >> rShift, nowNode[2] >> rShift};
                    auto&              anc_Level  = m_Layers[nowLevel];

                    auto anc_feat = anc_Level.read(anc_coords[0], anc_coords[1], anc_coords[2]);

                    int anc_coords_octant = GetOctant(anc_coords[0], anc_coords[1], anc_coords[2]);

                    int anc_coords_occupy = GetOccupy(anc_coords[0], anc_coords[1], anc_coords[2], nowLevel);

                    hiks(i, sLevel, 0) = anc_coords_octant;
                    hiks(i, sLevel, 1) = anc_coords_occupy;
                    hiks(i, sLevel, 2) = nowLevel;
                }
            }

            return hiks;
        }

        int GetOctant(int x, int y, int z)
        {
            int octant = ((x & 0x1) << 0) | //
                         ((y & 0x1) << 1) | //
                         ((z & 0x1) << 2);

            // std::cout << octant << std::endl;
            return octant;
        }

        int GetOccupy(int x, int y, int z, uint32_t level)
        {
            int occupy = 0;

            for (int bbz = 0; bbz <= 1; ++bbz)
            {
                for (int bby = 0; bby <= 1; ++bby)
                {
                    for (int bbx = 0; bbx <= 1; ++bbx)
                    {
                        std::array<int, 3> occ_coord = {
                            (x << 1) | bbx, //
                            (y << 1) | bby, //
                            (z << 1) | bbz  //
                        };

                        bool occ_coord_has = m_Layers[level + 1].has(occ_coord[0], occ_coord[1], occ_coord[2]);

                        occupy = occupy << 1;
                        occupy = occupy | (int)occ_coord_has;
                        // std::cout << occupy << std::endl;
                    }
                }
            }

            return occupy;
        }

    private:
        std::vector<OctreeLayer> m_Layers;
        std::vector<int>         m_Scales;
        uint32_t                 m_MaxLevel;

        std::vector<std::vector<std::array<int, 3>>> m_LevelNodes;
    };
} // namespace Engine
