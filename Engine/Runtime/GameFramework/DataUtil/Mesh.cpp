#include <Engine/Runtime/GameFramework/DataUtil/Mesh.h>

namespace Engine
{
    Mesh::Mesh() { m_VertexArray = VertexArray::Create(); }

    Mesh::Mesh(const void*        vertices,
               uint32_t*          indices,
               const uint32_t     vertice_count,
               const uint32_t     attribute_count,
               const uint32_t     indice_count,
               const BufferLayout layout)
    {
        Ref<VertexBuffer> vertexBuffer =
            VertexBuffer::Create(vertices, vertice_count * attribute_count * sizeof(float), vertice_count);
        vertexBuffer->SetLayout(layout);

        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indice_count);

        m_VertexArray = VertexArray::Create();
        m_VertexArray->AddVertexBuffer(vertexBuffer, false);
        m_VertexArray->AddIndexBuffer(indexBuffer);
    }

    void Mesh::AddTexture(const Ref<Texture2D>& texture) { m_Textures.push_back(texture); }

    void Mesh::AddVertexBuffer(const void*        vertices_array,
                               const uint32_t     vertice_count,
                               const uint32_t     attribute_count,
                               const uint32_t     attribute_size,
                               const BufferLayout layout)
    {
        Ref<VertexBuffer> vertexBuffer =
            VertexBuffer::Create(vertices_array, vertice_count * attribute_count * attribute_size, vertice_count);
        vertexBuffer->SetLayout(layout);

        m_VertexArray->AddVertexBuffer(vertexBuffer, false);
    }

    void Mesh::AddIndexBuffer(const uint32_t* indices, const uint32_t indice_count)
    {
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indice_count);
        m_VertexArray->AddIndexBuffer(indexBuffer);
    }
}; // namespace Engine