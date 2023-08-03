#include <Engine/Runtime/GameFramework/DataUtil/Mesh.h>

namespace Engine
{
    Mesh::Mesh() {}

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

    Mesh* Mesh::Merge(Mesh* mesh1, const Mesh* mesh2)
    {
        const uint32_t vertice_count_mesh1 = mesh1->m_Vertices.size();
        const uint32_t indice_count_mesh1  = mesh1->m_Indices.size();

        mesh1->m_Vertices.insert(mesh1->m_Vertices.end(), mesh2->m_Vertices.begin(), mesh2->m_Vertices.end());
        mesh1->m_Indices.insert(mesh1->m_Indices.end(), mesh2->m_Indices.begin(), mesh2->m_Indices.end());

        for (int i = indice_count_mesh1; i < mesh1->m_Indices.size(); i++)
        {
            mesh1->m_Indices[i] += vertice_count_mesh1;
        }

        mesh1->GenerateFrustumVolume();

        return mesh1;
    }

    uint32_t Mesh::GetVerticeSize() const { return m_Vertices.size(); }
    uint32_t Mesh::GetIndiceSize() const { return m_Indices.size(); }

    void Mesh::AddVertex(const Vertex& vertex) { m_Vertices.push_back(vertex); }

    void Mesh::AddIndex(const uint32_t& index) { m_Indices.push_back(index); }

    void Mesh::CreateBuffer(const BufferLayout& layout)
    {
        uint32_t vertice_count    = m_Vertices.size();
        uint32_t attribute_stride = layout.GetStride();

        Ref<VertexBuffer> vertexBuffer =
            VertexBuffer::Create(m_Vertices.data(), vertice_count * attribute_stride, vertice_count);
        vertexBuffer->SetLayout(layout);

        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());

        m_VertexArray = VertexArray::Create();
        m_VertexArray->AddVertexBuffer(vertexBuffer, false);
        m_VertexArray->AddIndexBuffer(indexBuffer);
    }

    glm::vec3 Mesh::GetCenter() { return m_FrustumVolume.center; }

    void Mesh::GenerateFrustumVolume()
    {
        glm::vec3 max_pos(FLT_MIN);
        glm::vec3 min_pos(FLT_MAX);

        for (int i = 0; i < m_Vertices.size(); i++)
        {
            max_pos = glm::max(max_pos, m_Vertices[i].position);
            min_pos = glm::min(min_pos, m_Vertices[i].position);
        }

        m_FrustumVolume = FrustumAABB {min_pos, max_pos};
    }

    FrustumVolume* Mesh::GetFrustumVolume() { return &m_FrustumVolume; }

    void Mesh::SetFrustumAABB(const FrustumAABB& volume) { m_FrustumVolume = volume; }
}; // namespace Engine