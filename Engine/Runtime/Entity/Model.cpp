#include "Model.h"

namespace Engine {
Mesh::Mesh() { m_VertexArray = VertexArray::Create(); }

Mesh::Mesh(const void* vertices, uint32_t* indices,
           const uint32_t vertice_count, const uint32_t attribute_count,
           const uint32_t indice_count, const BufferLayout layout) {
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

void Mesh::AddVertexBuffer(const void* vertices_array,
                           const uint32_t vertice_count,
                           const uint32_t attribute_count,
                           const uint32_t attribute_size,
                           const BufferLayout layout) {
    Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(
        vertices_array, vertice_count * attribute_count * attribute_size,
        vertice_count);
    vertexBuffer->SetLayout(layout);

    m_VertexArray->AddVertexBuffer(vertexBuffer, false);
}

void Mesh::AddIndexBuffer(const uint32_t* indices,
                          const uint32_t indice_count) {
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indice_count);
    m_VertexArray->AddIndexBuffer(indexBuffer);
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

    m_Meshes[0].AddTexture(
        Texture2D::Create("Assert/Object/checkerboard/Checkerboard.png"));
}

Model::Model(const std::string& path)
    : m_Directory(path.substr(0, path.find_last_of('/'))) {
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

    for (auto& mesh : m_Meshes) {
        if (mesh.m_Textures.size() == 0) {
            mesh.AddTexture(Texture2D::Create(
                "Assert/Object/checkerboard/Checkerboard.png"));
        }
    }
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

        if (amesh->mTextureCoords[0]) {
            vertices.push_back(amesh->mTextureCoords[0][i].x);
            vertices.push_back(amesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
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

    if (amesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[amesh->mMaterialIndex];
        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                "texture_diffuse");
        m_Textures.insert(m_Textures.end(), diffuseMaps.begin(),
                          diffuseMaps.end());
        if (diffuseMaps.size() != 0) {
            mesh.AddTexture(diffuseMaps[0]);
        }

        auto specularMaps = loadMaterialTextures(
            material, aiTextureType_SPECULAR, "texture_specular");
        m_Textures.insert(m_Textures.end(), specularMaps.begin(),
                          specularMaps.end());
        if (specularMaps.size() != 0) {
            mesh.AddTexture(specularMaps[0]);
        }
    }

    return mesh;
}

std::vector<Ref<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat,
                                                        aiTextureType type,
                                                        std::string typeName) {
    std::vector<Ref<Texture2D>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path(str.C_Str());
        path = m_Directory + "/" + path;
        Ref<Texture2D> texture = Texture2D::Create(path);
        textures.push_back(texture);
    }
    return textures;
}
}  // namespace Engine