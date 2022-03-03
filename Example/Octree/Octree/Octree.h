#pragma once
#include <array>
#include <bitset>
#include <cstdint>
#include <memory>
#include <queue>
#include <vector>

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
    Octree(const std::vector<std::array<uint32_t, 6> >& points,
           const uint32_t maxLevel);

    const std::vector<Ref<OctreeNode> >& GetLevelNodes(const uint32_t level) {
        return m_LevelNodes[level];
    }

    uint32_t GetMaxLevel() { return m_MaxLevel; }
    uint32_t GetNumNodes() { return m_NumNodes; }
    uint32_t GetNumLeafs() { return m_NumLeafs; }

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

    std::vector<Ref<OctreeNode> > m_LevelHead;
    std::vector<std::vector<Ref<OctreeNode> > > m_LevelNodes;
};
}  // namespace Engine
