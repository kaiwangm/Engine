#pragma once
#include <array>
#include <bitset>
#include <cstdint>
#include <memory>
#include <queue>
#include <vector>

#include "Spare.h"

namespace Engine {
template <typename T>
using Ref = std::shared_ptr<T>;

class OctreeNode {
   public:
    enum OctreeNodeType { None = 0, Branche = 1, Leaf = 2 };

   public:
    OctreeNode(uint32_t x, uint32_t y, uint32_t z, uint32_t r, uint32_t g,
               uint32_t b, uint32_t level, uint32_t octant, OctreeNodeType type)
        : m_x(x),
          m_y(y),
          m_z(z),
          m_r(r),
          m_g(g),
          m_b(b),
          m_Level(level),
          m_Octant(octant),
          m_Type(type),
          m_Occupancy("00000000") {}

    uint32_t GetX() { return m_x; }
    uint32_t GetY() { return m_y; }
    uint32_t GetZ() { return m_z; }
    uint32_t GetR() { return m_r; }
    uint32_t GetG() { return m_g; }
    uint32_t GetB() { return m_b; }
    uint32_t GetLevel() { return m_Level; }
    uint32_t GetOctant() { return m_Octant; }

    void SetParent(const Ref<OctreeNode>& parent) { m_Parent = parent; }
    void SetNextNeighbor(const Ref<OctreeNode>& nex) { m_NextNeighbor = nex; }
    void SetPrevNeighbor(const Ref<OctreeNode>& pre) { m_PrevNeighbor = pre; }
    Ref<OctreeNode> GetParent() { return m_Parent; }
    Ref<OctreeNode> GetNextNeighbor() { return m_NextNeighbor; }
    Ref<OctreeNode> GetPrevNeighbor() { return m_PrevNeighbor; }

    void SetChildren(const Ref<OctreeNode>& children, const uint32_t& idx) {
        m_Childrens[idx] = children;
        m_Occupancy.set(idx, 1);
    }
    Ref<OctreeNode> GetChildren(const uint32_t& idx) {
        return m_Childrens[idx];
    }

   private:
    uint32_t m_x, m_y, m_z;
    uint32_t m_r, m_g, m_b;
    uint32_t m_Level;
    uint32_t m_Octant;

    OctreeNodeType m_Type;

    Ref<OctreeNode> m_Parent;
    Ref<OctreeNode> m_NextNeighbor;
    Ref<OctreeNode> m_PrevNeighbor;

    std::bitset<8> m_Occupancy;
    std::array<Ref<OctreeNode>, 8> m_Childrens;
};

class Octree {
   public:
    Octree(const uint32_t maxLevel);
    Octree(const std::vector<std::array<int, 3>>& coords,
           const std::vector<std::array<uint32_t, 3>>& feats,
           const uint32_t maxLevel);

    const std::vector<Ref<OctreeNode>> GetLevelNodes(const uint32_t level) {
        return m_LevelNodes[level];
    }

    uint32_t GetMaxLevel() { return m_MaxLevel; }
    uint32_t GetNumNodes() { return m_NumNodes; }
    uint32_t GetNumLeafs() { return m_NumLeafs; }

    uint32_t GetLevelNumNodes(const uint32_t& level) {
        return (uint32_t)m_LevelNodes[level].size();
    }

   private:
    void insert(uint32_t x, uint32_t y, uint32_t z, uint32_t r, uint32_t g,
                uint32_t b);
    void Update();

   private:
    uint32_t m_MaxLevel;
    uint32_t m_MaxBound;
    Ref<OctreeNode> m_Root;

    uint32_t m_NumNodes = 0;
    uint32_t m_NumLeafs = 0;

    std::vector<Ref<OctreeNode>> m_LevelHead;
    std::vector<std::vector<Ref<OctreeNode>>> m_LevelNodes;
};

template <class T>
class HashOctree {
    using OctreeLayer = RootBlock3D<
        T, HashBlock3D<PointerBlock3D<2, DenseBlock3D<2, LeafData<T>>>>>;

   public:
    HashOctree(const std::vector<std::array<int, 3>>& coords,
               const std::vector<T>& feats, const uint32_t maxLevel)
        : m_Layers(coords.size()),
          m_MaxLevel(maxLevel),
          m_Scales(maxLevel + 1) {
        for (int l = 0; l <= maxLevel; ++l) {
            m_Scales[l] = 1 << (m_MaxLevel - l);
            for (int idx = 0; idx < coords.size(); ++idx) {
                int lshift = (m_MaxLevel - l);
                int x = coords[idx][0] >> lshift;
                int y = coords[idx][1] >> lshift;
                int z = coords[idx][2] >> lshift;
                m_Layers[l].write(x, y, z, feats[idx]);
            }

            std::vector<std::array<int, 3>> LevelNode;
            m_Layers[l].foreach ([l, &LevelNode](int x, int y, int z, T& val) {
                LevelNode.push_back({x, y, z});
            });
            m_LevelNodes.push_back(LevelNode);
        }
    }

    const std::tuple<std::vector<std::array<int, 3>>, std::vector<T>>
    GetLevelNodes(const uint32_t level) {
        std::vector<std::array<int, 3>> coords = m_LevelNodes[level];
        std::vector<T> feats;
        for (const auto& co : coords) {
            feats.push_back(m_Layers[level].read(co[0], co[1], co[2]));
        }

        return {coords, feats};
    }

    uint32_t GetMaxLevel() { return m_MaxLevel; }
    uint32_t GetNumNodes() {
        uint32_t count = 0;
        for (const auto& lever : m_LevelNodes) {
            count += (uint32_t)lever.size();
        }
        return count;
    }
    uint32_t GetNumLeafs() { return (uint32_t)m_LevelNodes[m_MaxLevel].size(); }

    uint32_t GetLevelNumNodes(const uint32_t& level) {
        return (uint32_t)m_LevelNodes[level].size();
    }

    uint32_t GetScale(const uint32_t& level) { return m_Scales[level]; }

   private:
    std::vector<OctreeLayer> m_Layers;
    std::vector<int> m_Scales;
    uint32_t m_MaxLevel;

    std::vector<std::vector<std::array<int, 3>>> m_LevelNodes;
};
}  // namespace Engine
