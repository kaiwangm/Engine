#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Renderer/Renderer.h>
#include <Engine/Runtime/GameFramework/Camera/Frustum.h>

namespace Engine
{
    struct Mesh
    {
        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texcoord;
            glm::vec3 tangent;
            glm::vec3 bitangent;
        };

        std::vector<Vertex>   m_Vertices;
        std::vector<uint32_t> m_Indices;

        Ref<VertexArray> m_VertexArray;
        FrustumAABB      m_FrustumVolume;

        Mesh();
        Mesh(const void*        vertices,
             uint32_t*          indices,
             const uint32_t     vertice_count,
             const uint32_t     attribute_count,
             const uint32_t     indice_count,
             const BufferLayout layout);

        static Mesh* Merge(Mesh* mesh1, const Mesh* mesh2);

        uint32_t GetVerticeSize() const;
        uint32_t GetIndiceSize() const;

        void AddVertex(const Vertex& vertex);
        void AddIndex(const uint32_t& index);
        void CreateBuffer(const BufferLayout& layout);

        glm::vec3 GetCenter();

        void           GenerateFrustumVolume();
        FrustumVolume* GetFrustumVolume();
        void           SetFrustumAABB(const FrustumAABB& volume);
    };
}; // namespace Engine