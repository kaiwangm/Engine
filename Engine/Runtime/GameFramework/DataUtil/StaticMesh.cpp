#include <Engine/Runtime/GameFramework/DataUtil/StaticMesh.h>
#include <Engine/Runtime/GameFramework/Material/MBasicPbr.h>

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

        m_Meshes.push_back(new Mesh(vertices,
                                    indices,
                                    4,
                                    8,
                                    6,
                                    {
                                        {0, ShaderDataType::Float3, "a_Position"},
                                        {1, ShaderDataType::Float3, "a_Normal"},
                                        {2, ShaderDataType::Float2, "a_TexCoord"},
                                    }));

        m_Meshes[0]->AddTexture(Texture2D::Create("Assets/Editor/Object/checkerboard/Checkerboard.png"));
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

        for (int i = 0; i < m_Materials.size(); i++)
        {
            if (m_Materials[i] == nullptr)
            {
                MBasicPbr* material = new MBasicPbr("Default");
                m_Materials[i]      = material;
                material->SetWorkflow(0);
            }
        }
    }

    void StaticMesh::processNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    void StaticMesh::processMesh(aiMesh* amesh, const aiScene* scene)
    {
        std::vector<float>    vertices;
        std::vector<uint32_t> indices;
        // std::vector<Texture> textures;

        Log::Info(fmt::format("Mesh Name: {0}", amesh->mName.C_Str()));
        Log::Info(fmt::format("Mesh NumVertices: {0}", amesh->mNumVertices));
        Log::Info(fmt::format("Mesh NumFaces: {0}", amesh->mNumFaces));

        glm::vec3 center(0.0f);
        glm::vec3 max_pos(FLT_MIN);
        glm::vec3 min_pos(FLT_MAX);

        for (unsigned int i = 0; i < amesh->mNumVertices; i++)
        {
            vertices.push_back(amesh->mVertices[i].x);
            vertices.push_back(amesh->mVertices[i].y);
            vertices.push_back(amesh->mVertices[i].z);

            center += glm::vec3(amesh->mVertices[i].x, amesh->mVertices[i].y, amesh->mVertices[i].z);
            max_pos = glm::max(max_pos, glm::vec3(amesh->mVertices[i].x, amesh->mVertices[i].y, amesh->mVertices[i].z));
            min_pos = glm::min(min_pos, glm::vec3(amesh->mVertices[i].x, amesh->mVertices[i].y, amesh->mVertices[i].z));

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

        Mesh* mesh = new Mesh(vertices.data(),
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

        if (amesh->mNumVertices > 0)
        {
            center /= (float)amesh->mNumVertices;
        }
        float radius = glm::distance(max_pos, min_pos) / 2.0f;

        mesh->SetFrustumAABB({min_pos, max_pos});

        MBasicPbr* material = nullptr;

        if (amesh->mMaterialIndex > 0)
        {
            aiMaterial* mat = scene->mMaterials[amesh->mMaterialIndex];
            Log::Info(fmt::format("Material Name: {0}", mat->GetName().C_Str()));
            std::string materialName = mat->GetName().C_Str();

            if (m_LoadedMaterials.find(materialName) != m_LoadedMaterials.end())
            {
                material = static_cast<MBasicPbr*>(m_LoadedMaterials[materialName]);
            }
            else
            {
                material = new MBasicPbr(materialName);
                material->SetWorkflow(1);

                aiColor3D diffuse;
                mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
                material->SetAlbedo(glm::vec3(diffuse.r, diffuse.g, diffuse.b));

                aiColor3D specular;
                mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
                material->SetSpecular(glm::vec3(specular.r, specular.g, specular.b));

                float shininess;
                mat->Get(AI_MATKEY_SHININESS, shininess);
                float roughness = glm::clamp(1.0f - shininess / 120.0f, 0.05f, 0.97f);
                material->SetRoughness(roughness);

                Log::Info(fmt::format("Material Diffuse: {0}, {1}, {2}", diffuse.r, diffuse.g, diffuse.b));
                Log::Info(fmt::format("Material Specular: {0}, {1}, {2}", specular.r, specular.g, specular.b));
                Log::Info(fmt::format("Material Roughness: {0}", roughness));

                aiString diffuseTexturePath;
                if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath) == AI_SUCCESS)
                {
                    std::string texturePath = m_Directory + "/" + diffuseTexturePath.C_Str();
                    Log::Info(fmt::format("Material Diffuse Texture Path: {0}", texturePath));
                    material->LoadDiffuseMap(texturePath);
                }

                aiString normalTexturePath;
                if (mat->GetTexture(aiTextureType_NORMALS, 0, &normalTexturePath) == AI_SUCCESS)
                {
                    std::string texturePath = m_Directory + "/" + normalTexturePath.C_Str();
                    Log::Info(fmt::format("Material Normal Texture Path: {0}", texturePath));
                    material->LoadNormalMap(texturePath);
                }

                aiString specularTexturePath;
                if (mat->GetTexture(aiTextureType_SPECULAR, 0, &specularTexturePath) == AI_SUCCESS)
                {
                    std::string texturePath = m_Directory + "/" + specularTexturePath.C_Str();
                    Log::Info(fmt::format("Material Specular Texture Path: {0}", texturePath));
                    material->LoadSpecularMap(texturePath);
                }

                m_LoadedMaterials[materialName] = material;
            }
        }

        m_Meshes.push_back(mesh);
        m_Materials.push_back(material);
    }
} // namespace Engine