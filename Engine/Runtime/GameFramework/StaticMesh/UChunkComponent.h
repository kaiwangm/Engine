#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Renderer/Renderer.h>
#include <Engine/Runtime/GameFramework/DataUtil/Mesh.h>

#include <vector>

namespace Engine
{
    class UChunkComponent final : public UComponent
    {
    public:
        constexpr static uint32_t ChunkSize   = 32;
        constexpr static uint32_t ChunkArea   = ChunkSize * ChunkSize;
        constexpr static uint32_t ChunkVolume = ChunkSize * ChunkSize * ChunkSize;

    private:
        struct Vertex
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 TexCoord;
            glm::vec3 Tangent;
            glm::vec3 Bitangent;
            uint32_t  MaterialIndex;
            uint32_t  FaceIndex;
        };

        const BufferLayout m_Layout {
            {0, ShaderDataType::Float3, "a_Position"},
            {1, ShaderDataType::Float3, "a_Normal"},
            {2, ShaderDataType::Float2, "a_TexCoord"},
            {3, ShaderDataType::Float3, "a_Tangent"},
            {4, ShaderDataType::Float3, "a_Bitangent"},
            {5, ShaderDataType::Int, "a_MaterialIndex"},
            {6, ShaderDataType::Int, "a_FaceIndex"},
        };

        uint32_t            m_Voxels[ChunkSize][ChunkSize][ChunkSize];
        std::vector<Vertex> m_Vertices;
        bool                m_IsDirty = false;

        Ref<VertexArray>      m_VertexArray;
        static Ref<Texture2D> m_Texture;

    private:
        UChunkComponent* m_TopChunkPointer    = nullptr;
        UChunkComponent* m_BottomChunkPointer = nullptr;
        UChunkComponent* m_LeftChunkPointer   = nullptr;
        UChunkComponent* m_RightChunkPointer  = nullptr;
        UChunkComponent* m_FrontChunkPointer  = nullptr;
        UChunkComponent* m_BackChunkPointer   = nullptr;

    public:
        void SetTopChunkPointer(UChunkComponent* chunk) { m_TopChunkPointer = chunk; }
        void SetBottomChunkPointer(UChunkComponent* chunk) { m_BottomChunkPointer = chunk; }
        void SetLeftChunkPointer(UChunkComponent* chunk) { m_LeftChunkPointer = chunk; }
        void SetRightChunkPointer(UChunkComponent* chunk) { m_RightChunkPointer = chunk; }
        void SetFrontChunkPointer(UChunkComponent* chunk) { m_FrontChunkPointer = chunk; }
        void SetBackChunkPointer(UChunkComponent* chunk) { m_BackChunkPointer = chunk; }

        void BuildVertexData()
        {
            static const glm::vec3 vertices[8] = {
                {0.0f, 0.0f, 0.0f},
                {1.0f, 0.0f, 0.0f},
                {1.0f, 0.0f, 1.0f},
                {0.0f, 0.0f, 1.0f},
                {0.0f, 1.0f, 0.0f},
                {1.0f, 1.0f, 0.0f},
                {1.0f, 1.0f, 1.0f},
                {0.0f, 1.0f, 1.0f},
            };

            /*
              7 - 6       5
            4 - 5 |     4  3
            | 3 | 2      0
            0 - 1       1  2
            */

            std::vector<glm::ivec3> vertices_id;
            std::vector<glm::vec3>  vertices_bias;
            std::vector<int>        faces_id;

            for (uint32_t x = 0; x < ChunkSize; x++)
            {
                for (uint32_t y = 0; y < ChunkSize; y++)
                {
                    for (uint32_t z = 0; z < ChunkSize; z++)
                    {
                        uint32_t voxel_if = m_Voxels[x][y][z];
                        if (voxel_if == 0)
                        {
                            continue;
                        }

                        if (isVoid(x, y + 1, z))
                        {
                            vertices_id.push_back({4, 7, 6});
                            faces_id.push_back(5);
                            vertices_id.push_back({4, 6, 5});
                            faces_id.push_back(5);
                            vertices_bias.push_back({x, y, z});
                        }
                        if (isVoid(x, y - 1, z))
                        {
                            vertices_id.push_back({0, 1, 2});
                            faces_id.push_back(0);
                            vertices_id.push_back({0, 2, 3});
                            faces_id.push_back(0);
                            vertices_bias.push_back({x, y, z});
                        }
                        if (isVoid(x + 1, y, z))
                        {
                            vertices_id.push_back({1, 5, 6});
                            faces_id.push_back(2);
                            vertices_id.push_back({1, 6, 2});
                            faces_id.push_back(2);
                            vertices_bias.push_back({x, y, z});
                        }
                        if (isVoid(x - 1, y, z))
                        {
                            vertices_id.push_back({3, 7, 4});
                            faces_id.push_back(4);
                            vertices_id.push_back({3, 4, 0});
                            faces_id.push_back(4);
                            vertices_bias.push_back({x, y, z});
                        }
                        if (isVoid(x, y, z + 1))
                        {
                            vertices_id.push_back({2, 6, 7});
                            faces_id.push_back(1);
                            vertices_id.push_back({2, 7, 3});
                            faces_id.push_back(1);
                            vertices_bias.push_back({x, y, z});
                        }
                        if (isVoid(x, y, z - 1))
                        {
                            vertices_id.push_back({0, 4, 5});
                            faces_id.push_back(3);
                            vertices_id.push_back({0, 5, 1});
                            faces_id.push_back(3);
                            vertices_bias.push_back({x, y, z});
                        }
                    }
                }
            }

            m_Vertices.clear();
            for (int i = 0; i < vertices_bias.size(); i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Vertex vertex;
                    vertex.Position = vertices[vertices_id[i * 2][j]] + vertices_bias[i];
                    vertex.MaterialIndex =
                        m_Voxels[(int)vertices_bias[i].x][(int)vertices_bias[i].y][(int)vertices_bias[i].z];
                    vertex.FaceIndex = faces_id[i * 2];

                    if (vertex.FaceIndex == 0)
                    {
                        vertex.Normal    = glm::vec3(0.0f, -1.0f, 0.0f);
                        vertex.Tangent   = glm::vec3(1.0f, 0.0f, 0.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 0.0f, -1.0f);
                    }
                    else if (vertex.FaceIndex == 1)
                    {
                        vertex.Normal    = glm::vec3(0.0f, 0.0f, 1.0f);
                        vertex.Tangent   = glm::vec3(1.0f, 0.0f, 0.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    else if (vertex.FaceIndex == 2)
                    {
                        vertex.Normal    = glm::vec3(1.0f, 0.0f, 0.0f);
                        vertex.Tangent   = glm::vec3(0.0f, 0.0f, 1.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    else if (vertex.FaceIndex == 3)
                    {
                        vertex.Normal    = glm::vec3(0.0f, 0.0f, -1.0f);
                        vertex.Tangent   = glm::vec3(-1.0f, 0.0f, 0.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    else if (vertex.FaceIndex == 4)
                    {
                        vertex.Normal    = glm::vec3(-1.0f, 0.0f, 0.0f);
                        vertex.Tangent   = glm::vec3(0.0f, 0.0f, -1.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    else if (vertex.FaceIndex == 5)
                    {
                        vertex.Normal    = glm::vec3(0.0f, 1.0f, 0.0f);
                        vertex.Tangent   = glm::vec3(-1.0f, 0.0f, 0.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }

                    if (j == 0)
                    {
                        vertex.TexCoord = glm::vec2(1.0f, 1.0f);
                    }
                    else if (j == 1)
                    {
                        vertex.TexCoord = glm::vec2(1.0f, 0.0f);
                    }
                    else if (j == 2)
                    {
                        vertex.TexCoord = glm::vec2(0.0f, 0.0f);
                    }
                    m_Vertices.push_back(vertex);
                }
                for (int j = 0; j < 3; j++)
                {
                    Vertex vertex;
                    vertex.Position = vertices[vertices_id[i * 2 + 1][j]] + vertices_bias[i];
                    vertex.MaterialIndex =
                        m_Voxels[(int)vertices_bias[i].x][(int)vertices_bias[i].y][(int)vertices_bias[i].z];
                    vertex.FaceIndex = faces_id[i * 2 + 1];

                    if (vertex.FaceIndex == 0)
                    {
                        vertex.Normal    = glm::vec3(0.0f, -1.0f, 0.0f);
                        vertex.Tangent   = glm::vec3(1.0f, 0.0f, 0.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 0.0f, -1.0f);
                    }
                    else if (vertex.FaceIndex == 1)
                    {
                        vertex.Normal    = glm::vec3(0.0f, 0.0f, 1.0f);
                        vertex.Tangent   = glm::vec3(1.0f, 0.0f, 0.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    else if (vertex.FaceIndex == 2)
                    {
                        vertex.Normal    = glm::vec3(1.0f, 0.0f, 0.0f);
                        vertex.Tangent   = glm::vec3(0.0f, 0.0f, 1.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    else if (vertex.FaceIndex == 3)
                    {
                        vertex.Normal    = glm::vec3(0.0f, 0.0f, -1.0f);
                        vertex.Tangent   = glm::vec3(-1.0f, 0.0f, 0.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    else if (vertex.FaceIndex == 4)
                    {
                        vertex.Normal    = glm::vec3(-1.0f, 0.0f, 0.0f);
                        vertex.Tangent   = glm::vec3(0.0f, 0.0f, -1.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    else if (vertex.FaceIndex == 5)
                    {
                        vertex.Normal    = glm::vec3(0.0f, 1.0f, 0.0f);
                        vertex.Tangent   = glm::vec3(-1.0f, 0.0f, 0.0f);
                        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
                    }

                    if (j == 0)
                    {
                        vertex.TexCoord = glm::vec2(1.0f, 1.0f);
                    }
                    else if (j == 1)
                    {
                        vertex.TexCoord = glm::vec2(0.0f, 0.0f);
                    }
                    else if (j == 2)
                    {
                        vertex.TexCoord = glm::vec2(0.0f, 1.0f);
                    }
                    m_Vertices.push_back(vertex);
                }
            }

            Ref<VertexBuffer> vertexBuffer =
                VertexBuffer::Create(m_Vertices.data(), m_Vertices.size() * m_Layout.GetStride(), m_Vertices.size());
            vertexBuffer->SetLayout(m_Layout);

            m_VertexArray->AddVertexBuffer(vertexBuffer, false);
        }

    public:
        UChunkComponent()
        {
            if (m_Texture == nullptr)
            {
                m_Texture = Texture2D::Create("Assets/Voxelcraft/Texture/tex_array_0.png");
            }

            for (uint32_t x = 0; x < ChunkSize; x++)
            {
                for (uint32_t y = 0; y < ChunkSize; y++)
                {
                    for (uint32_t z = 0; z < ChunkSize; z++)
                    {
                        m_Voxels[x][y][z] = 0;
                    }
                }
            }

            m_VertexArray = VertexArray::Create();
        }

        ~UChunkComponent() {}

        bool isVoid(int x, int y, int z) const
        {
            if (x < 0 || y < 0 || z < 0 || x >= ChunkSize || y >= ChunkSize || z >= ChunkSize)
            {
                if (x < 0)
                {
                    if (m_LeftChunkPointer != nullptr)
                    {
                        return m_LeftChunkPointer->isVoid(x + ChunkSize, y, z);
                    }
                }
                else if (x >= ChunkSize)
                {
                    if (m_RightChunkPointer != nullptr)
                    {
                        return m_RightChunkPointer->isVoid(x - ChunkSize, y, z);
                    }
                }
                else if (y < 0)
                {
                    if (m_BottomChunkPointer != nullptr)
                    {
                        return m_BottomChunkPointer->isVoid(x, y + ChunkSize, z);
                    }
                }
                else if (y >= ChunkSize)
                {
                    if (m_TopChunkPointer != nullptr)
                    {
                        return m_TopChunkPointer->isVoid(x, y - ChunkSize, z);
                    }
                }
                else if (z < 0)
                {
                    if (m_FrontChunkPointer != nullptr)
                    {
                        return m_FrontChunkPointer->isVoid(x, y, z + ChunkSize);
                    }
                }
                else if (z >= ChunkSize)
                {
                    if (m_BackChunkPointer != nullptr)
                    {
                        return m_BackChunkPointer->isVoid(x, y, z - ChunkSize);
                    }
                }

                return true;
            }

            return m_Voxels[x][y][z] == 0;
        }

        void TickLogic(float deltaTime)
        {
            if (m_IsDirty)
            {
                BuildVertexData();
                m_IsDirty = false;
            }
        }

        uint32_t     GetVertexCount() const { return m_Vertices.size(); }
        VertexArray& GetVertexArrayRef() const { return *m_VertexArray; }
        Texture2D&   GetTextureRef() const { return *m_Texture; }

        uint32_t* GetVoxelPtr(int x, int y, int z)
        {
            if (x < 0 || y < 0 || z < 0 || x >= ChunkSize || y >= ChunkSize || z >= ChunkSize)
            {
                return nullptr;
            }

            return &m_Voxels[x][y][z];
        }

        void SetDirty() { m_IsDirty = true; }

        void SetVoxel(int x, int y, int z, uint32_t voxel)
        {
            if (x < 0 || y < 0 || z < 0 || x >= ChunkSize || y >= ChunkSize || z >= ChunkSize)
            {
                return;
            }

            m_Voxels[x][y][z] = voxel;
            m_IsDirty         = true;

            if (m_LeftChunkPointer != nullptr)
            {
                m_LeftChunkPointer->SetDirty();
            }

            if (m_RightChunkPointer != nullptr)
            {
                m_RightChunkPointer->SetDirty();
            }

            if (m_BottomChunkPointer != nullptr)
            {
                m_BottomChunkPointer->SetDirty();
            }

            if (m_TopChunkPointer != nullptr)
            {
                m_TopChunkPointer->SetDirty();
            }

            if (m_FrontChunkPointer != nullptr)
            {
                m_FrontChunkPointer->SetDirty();
            }

            if (m_BackChunkPointer != nullptr)
            {
                m_BackChunkPointer->SetDirty();
            }
        }

        uint32_t GetVoxel(int x, int y, int z) const
        {
            if (x < 0 || y < 0 || z < 0 || x >= ChunkSize || y >= ChunkSize || z >= ChunkSize)
            {
                return 0;
            }

            return m_Voxels[x][y][z];
        }
    };
}; // namespace Engine