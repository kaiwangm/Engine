#include <Engine/Runtime/GameFramework/DataUtil/StaticMesh.h>

namespace Engine
{
    StaticMesh::StaticMesh()
    {
        float vertices[4 * 9] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //
            0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //
            -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //
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

        m_Meshes[0].AddTexture(Texture2D::Create("Assets/Editor/Object/checkerboard/Checkerboard.png"));
    }

    StaticMesh::StaticMesh(const std::string& path) : m_Directory(path.substr(0, path.find_last_of('/')))
    {
        Assimp::Importer import;
        const aiScene*   scene =
            import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Log::Core_Error(fmt::format("ERROR::ASSIMP::{0}", std::string(import.GetErrorString())));
            return;
        }

        processNode(scene->mRootNode, scene);

        for (auto& mesh : m_Meshes)
        {
            if (mesh.m_Textures.size() == 0)
            {
                mesh.AddTexture(Texture2D::Create("Assets/Editor/Object/checkerboard/Checkerboard.png"));
            }
        }
    }

    void StaticMesh::processNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh StaticMesh::processMesh(aiMesh* amesh, const aiScene* scene)
    {
        std::vector<float>    vertices;
        std::vector<uint32_t> indices;
        // std::vector<Texture> textures;

        for (unsigned int i = 0; i < amesh->mNumVertices; i++)
        {
            vertices.push_back(amesh->mVertices[i].x);
            vertices.push_back(amesh->mVertices[i].y);
            vertices.push_back(amesh->mVertices[i].z);

            vertices.push_back(amesh->mNormals[i].x);
            vertices.push_back(amesh->mNormals[i].y);
            vertices.push_back(amesh->mNormals[i].z);

            if (amesh->mTextureCoords[0])
            {
                vertices.push_back(amesh->mTextureCoords[0][i].x);
                vertices.push_back(amesh->mTextureCoords[0][i].y);

                vertices.push_back(amesh->mTangents[i].x);
                vertices.push_back(amesh->mTangents[i].y);
                vertices.push_back(amesh->mTangents[i].z);

                vertices.push_back(amesh->mBitangents[i].x);
                vertices.push_back(amesh->mBitangents[i].y);
                vertices.push_back(amesh->mBitangents[i].z);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);

                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);

                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }

        for (unsigned int i = 0; i < amesh->mNumFaces; i++)
        {
            aiFace face = amesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        Mesh mesh(vertices.data(),
                  indices.data(),
                  amesh->mNumVertices,
                  3 + 3 + 2 + 3 + 3,
                  amesh->mNumFaces * 3,
                  {
                      {0, ShaderDataType::Float3, "a_Position"},
                      {1, ShaderDataType::Float3, "a_Normal"},
                      {2, ShaderDataType::Float2, "a_TexCoord"},
                      {3, ShaderDataType::Float3, "a_Tangent"},
                      {4, ShaderDataType::Float3, "a_Bitangent"},
                  });

        if (amesh->mMaterialIndex >= 0)
        {
            aiMaterial* material    = scene->mMaterials[amesh->mMaterialIndex];
            auto        diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            m_Textures.insert(m_Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            if (diffuseMaps.size() != 0)
            {
                mesh.AddTexture(diffuseMaps[0]);
            }

            auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            m_Textures.insert(m_Textures.end(), specularMaps.begin(), specularMaps.end());
            if (specularMaps.size() != 0)
            {
                mesh.AddTexture(specularMaps[0]);
            }
        }

        return mesh;
    }

    std::vector<Ref<Texture2D>>
    StaticMesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Ref<Texture2D>> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string path(str.C_Str());
            path                   = m_Directory + "/" + path;
            Ref<Texture2D> texture = Texture2D::Create(path);
            textures.push_back(texture);
        }
        return textures;
    }
} // namespace Engine