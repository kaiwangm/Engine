#pragma once
#include <map>
#include <memory>
#include <unordered_map>

namespace Engine {
template <class T>
struct LeafDate {
    static constexpr bool isLeaf = true;

    static constexpr int LeafNum = 1;

    T m_Value;

    T read() { return m_Value; }

    void write(T value) { m_Value = value; }
};

template <int Dim, class Node>
struct DenseBlock3D {
    static constexpr bool isLeaf = false;

    static constexpr int B = Dim;
    static constexpr int LeafNum = B * Node::LeafNum;
    static constexpr int SubLeafNum = Node::LeafNum;

    Node m_Data[B][B][B];

    Node* fetch(int x, int y, int z) { return &m_Data[x][y][z]; }

    Node* touch(int x, int y, int z) { return &m_Data[x][y][z]; }
};

template <int Dim, class Node>
struct PointerBlock3D {
    static constexpr bool isLeaf = false;

    static constexpr int B = Dim;
    static constexpr int LeafNum = B * Node::LeafNum;
    static constexpr int SubLeafNum = Node::LeafNum;

    std::unique_ptr<Node> m_Data[B][B][B];

    Node* fetch(int x, int y, int z) { return m_Data[x][y][z].get(); }

    Node* touch(int x, int y, int z) {
        auto& block = m_Data[x][y][z];
        if (!block) block = std::make_unique<Node>();
        return block.get();
    }
};

template <class Node>
struct HashBlock3D {
    static constexpr bool isLeaf = false;

    static constexpr int SubLeafNum = Node::LeafNum;

    struct Hash3DXYZ {
        std::size_t operator()(const std::tuple<int, int, int>& key) const {
            std::hash<int> int_hasher;
            const auto& [x, y, z] = key;
            return int_hasher(x) ^ int_hasher(y) ^ int_hasher(z);
        }
    };

    std::unordered_map<std::tuple<int, int, int>, Node, Hash3DXYZ> m_Data;

    Node* fetch(int x, int y, int z) {
        auto it = m_Data.find(std::make_tuple(x, y, z));
        if (it == m_Data.end()) {
            return nullptr;
        }
        return &it->second;
    }

    Node* touch(int x, int y, int z) {
        auto it = m_Data.find(std::make_tuple(x, y, z));
        if (it == m_Data.end())
            return &m_Data.try_emplace(std::make_tuple(x, y, z)).first->second;
        return &it->second;
    }
};

template <class T, class Layout>
struct RootBlock3D {
    static constexpr bool isLeaf = false;
    Layout m_Root;

    template <class Node>
    T _read(Node* node, int x, int y, int z) {
        if (node == nullptr) {
            return T();
        }
        if constexpr (node->isLeaf) {
            return node->read();
        } else {
            int SLN = node->SubLeafNum;
            int nx = x / SLN;
            int ny = y / SLN;
            int nz = z / SLN;
            int mx = (x % SLN + SLN) % SLN;
            int my = (y % SLN + SLN) % SLN;
            int mz = (z % SLN + SLN) % SLN;
            auto* child = node->fetch(nx, ny, nz);
            return _read(child, mx, my, mz);
        }
    }

    T read(int x, int y, int z) { return _read(&m_Root, x, y, z); }

    template <class Node>
    void _write(Node* node, int x, int y, int z, T value) {
        if constexpr (node->isLeaf) {
            node->write(value);
            return;
        } else {
            int SLN = node->SubLeafNum;
            int nx = x / SLN;
            int ny = y / SLN;
            int nz = z / SLN;
            int mx = (x % SLN + SLN) % SLN;
            int my = (y % SLN + SLN) % SLN;
            int mz = (z % SLN + SLN) % SLN;
            auto* child = node->touch(nx, ny, nz);
            _write(child, mx, my, mz, value);
            return;
        }
    }

    void write(int x, int y, int z, T value) {
        _write(&m_Root, x, y, z, value);
        return;
    }
};

}  // namespace Engine