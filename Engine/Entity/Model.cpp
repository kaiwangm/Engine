#include "Model.h"

namespace Engine {
Mesh::Mesh(float* vertices, uint32_t* indices, const uint32_t vertice_count,
           const uint32_t attribute_count, const uint32_t indice_count,
           const BufferLayout layout) {
    Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(
        vertices, vertice_count * attribute_count * sizeof(float),
        vertice_count);
    vertexBuffer->SetLayout(layout);

    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indice_count);

    m_VertexArray = VertexArray::Create();
    m_VertexArray->AddVertexBuffer(vertexBuffer, false);
    m_VertexArray->AddIndexBuffer(indexBuffer);
}

void Mesh::AddTexture(const Ref<Texture2D>& texture) {
    m_Textures.push_back(texture);
}

Model::Model() {
    float vertices[4 * 9] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  //
        0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  //
        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  //
    };
    uint32_t indices[6] = {0, 1, 2, 2, 3, 0};

    m_Meshes.push_back({vertices,
                        indices,
                        4,
                        8,
                        6,
                        {
                            {0, ShaderDataType::Float3, "a_Position"},
                            {1, ShaderDataType::Float3, "a_Normal"},
                            {2, ShaderDataType::Float2, "a_TexCoord"},
                        }});

    m_Meshes[0].AddTexture(Texture2D::Create("Assert/Checkerboard.png"));
}

Model::Model(const std::string& path) {
    Assimp::Importer import;
    const aiScene* scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        Log::Core_Error("ERROR::ASSIMP::{0}",
                        std::string(import.GetErrorString()));
        return;
    }

    processNode(scene->mRootNode, scene);

    m_Meshes[0].AddTexture(Texture2D::Create("Assert/Checkerboard.png"));
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* amesh, const aiScene* scene) {
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    // std::vector<Texture> textures;

    for (unsigned int i = 0; i < amesh->mNumVertices; i++) {
        vertices.push_back(amesh->mVertices[i].x);
        vertices.push_back(amesh->mVertices[i].y);
        vertices.push_back(amesh->mVertices[i].z);

        vertices.push_back(amesh->mNormals[i].x);
        vertices.push_back(amesh->mNormals[i].y);
        vertices.push_back(amesh->mNormals[i].z);

        vertices.push_back(amesh->mTextureCoords[0][i].x);
        vertices.push_back(amesh->mTextureCoords[0][i].y);
    }

    for (unsigned int i = 0; i < amesh->mNumFaces; i++) {
        aiFace face = amesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Mesh mesh(vertices.data(), indices.data(), amesh->mNumVertices, 8,
              amesh->mNumFaces * 3,
              {
                  {0, ShaderDataType::Float3, "a_Position"},
                  {1, ShaderDataType::Float3, "a_Normal"},
                  {2, ShaderDataType::Float2, "a_TexCoord"},
              });

    return mesh;
}
}  // namespace Engine