#include "Octree.h"

namespace Engine
{
    Octree::Octree(const uint32_t maxLevel) :
        m_MaxLevel(maxLevel), m_MaxBound(1 << maxLevel), m_Root(nullptr), m_LevelHead(maxLevel + 1),
        m_LevelNodes(maxLevel + 1)
    {}

    Octree::Octree(const std::vector<std::array<int, 3>>&      coords,
                   const std::vector<std::array<uint32_t, 3>>& feats,
                   const uint32_t                              maxLevel) :
        m_MaxLevel(maxLevel),
        m_MaxBound(1 << maxLevel), m_Root(nullptr), m_LevelHead(maxLevel + 1), m_LevelNodes(maxLevel + 1)
    {
        for (int idx = 0; idx < coords.size(); ++idx)
        {
            this->insert(coords[idx][0], coords[idx][1], coords[idx][2], feats[idx][0], feats[idx][1], feats[idx][2]);
        }

        this->Update();
    }

    void Octree::insert(uint32_t x, uint32_t y, uint32_t z, uint32_t r, uint32_t g, uint32_t b)
    {
        std::vector<std::array<uint32_t, 3>> coords(m_MaxLevel + 1);
        std::vector<std::array<uint32_t, 3>> real_coords(m_MaxLevel + 1);
        std::vector<std::array<uint32_t, 3>> real_cols(m_MaxLevel + 1);
        for (uint32_t level = 0; level <= m_MaxLevel; ++level)
        {
            uint32_t div       = 1 << (m_MaxLevel - level);
            coords[level]      = {x / div, y / div, z / div};
            real_coords[level] = {
                coords[level][0] * div + div / 2, coords[level][1] * div + div / 2, coords[level][2] * div + div / 2};
            real_cols[level] = {r, g, b};
        }

        Ref<OctreeNode> ptr = m_Root;
        for (uint32_t level = 0; level <= m_MaxLevel; ++level)
        {
            OctreeNode::OctreeNodeType type =
                level == m_MaxLevel ? OctreeNode::OctreeNodeType::Leaf : OctreeNode::OctreeNodeType::Branche;

            std::array<uint32_t, 3> real_coord_cur = real_coords[level];
            std::array<uint32_t, 3> real_col_cur   = real_cols[level];
            if (level == 0)
            {
                if (ptr == nullptr)
                {
                    auto new_node = std::make_shared<OctreeNode>(real_coord_cur[0],
                                                                 real_coord_cur[1],
                                                                 real_coord_cur[2],
                                                                 real_col_cur[0],
                                                                 real_col_cur[1],
                                                                 real_col_cur[2],
                                                                 level,
                                                                 0,
                                                                 type);
                    m_Root        = new_node;
                    ptr           = new_node;

                    ++m_NumNodes;
                }
            }
            else
            {
                std::array<uint32_t, 3> coord_cur = coords[level];
                std::array<uint32_t, 3> coord_pre = coords[level - 1];

                std::array<uint32_t, 3> coord {
                    coord_cur[0] - coord_pre[0] * 2,
                    coord_cur[1] - coord_pre[1] * 2,
                    coord_cur[2] - coord_pre[2] * 2,
                };

                uint32_t octant  = coord[0] * 1 + coord[1] * 2 + coord[2] * 4;
                auto     nex_ptr = ptr->GetChildren(octant);

                if (nex_ptr == nullptr)
                {
                    auto new_node = std::make_shared<OctreeNode>(real_coord_cur[0],
                                                                 real_coord_cur[1],
                                                                 real_coord_cur[2],
                                                                 real_col_cur[0],
                                                                 real_col_cur[1],
                                                                 real_col_cur[2],
                                                                 level,
                                                                 octant,
                                                                 type);
                    ptr->SetChildren(new_node, octant);
                    new_node->SetParent(ptr);
                    nex_ptr = new_node;

                    ++m_NumNodes;
                    if (type == OctreeNode::OctreeNodeType::Leaf)
                    {
                        ++m_NumLeafs;
                    }
                }
                ptr = nex_ptr;
            }
        }
    }

    void Octree::Update()
    {
        std::queue<Ref<OctreeNode>> que;
        que.push(m_Root);

        Ref<OctreeNode> pre_ptr   = nullptr;
        uint32_t        now_level = 4294967295;
        while (!que.empty())
        {
            Ref<OctreeNode> ptr = que.front();
            que.pop();

            for (int i = 0; i < 8; ++i)
            {
                Ref<OctreeNode> children = ptr->GetChildren(i);
                if (children != nullptr)
                {
                    que.push(children);
                }
            }

            uint32_t level = ptr->GetLevel();
            if (level != now_level)
            {
                m_LevelHead[level] = ptr;
                now_level          = level;
                pre_ptr            = nullptr;
            }

            if (pre_ptr == nullptr)
            {
                ptr->SetPrevNeighbor(pre_ptr);
            }
            else
            {
                ptr->SetPrevNeighbor(pre_ptr);
                pre_ptr->SetNextNeighbor(ptr);
            }
            pre_ptr = ptr;
        }

        for (int i = 0; i < m_LevelHead.size(); ++i)
        {
            Ref<OctreeNode> ptr = m_LevelHead[i];
            while (ptr != nullptr)
            {
                m_LevelNodes[i].push_back(ptr);
                ptr = ptr->GetNextNeighbor();
            }
        }
    }
} // namespace Engine
