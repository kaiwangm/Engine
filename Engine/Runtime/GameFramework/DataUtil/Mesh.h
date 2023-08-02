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
        Ref<VertexArray>            m_VertexArray;
        std::vector<Ref<Texture2D>> m_Textures;
        FrustumAABB                 m_FrustumVolume;

        Mesh();
        Mesh(const void*        vertices,
             uint32_t*          indices,
             const uint32_t     vertice_count,
             const uint32_t     attribute_count,
             const uint32_t     indice_count,
             const BufferLayout layout);

        void AddTexture(const Ref<Texture2D>& texture);
        void AddVertexBuffer(const void*        vertices_array,
                             const uint32_t     vertice_count,
                             const uint32_t     attribute_count,
                             const uint32_t     attribute_size,
                             const BufferLayout layout);
        void AddIndexBuffer(const uint32_t* indices, const uint32_t indice_count);

        FrustumVolume* GetFrustumVolume();
        void           SetFrustumAABB(const FrustumAABB& volume);
    };
}; // namespace Engine