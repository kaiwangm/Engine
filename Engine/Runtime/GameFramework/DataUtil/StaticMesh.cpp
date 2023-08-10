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

        // m_Meshes[0]->AddTexture(Texture2D::Create("Assets/Editor/Object/checkerboard/Checkerboard.png"));
    }

    StaticMesh::StaticMesh(const std::string& path, const bool batching) :
        m_Directory(path.substr(0, path.find_last_of('/')))
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

        BufferLayout layout {
            {0, ShaderDataType::Float3, "a_Position"},
            {1, ShaderDataType::Float3, "a_Normal"},
            {2, ShaderDataType::Float2, "a_TexCoord"},
            {3, ShaderDataType::Float3, "a_Tangent"},
            {4, ShaderDataType::Float3, "a_Bitangent"},
        };

        // Merge meshes with same material
        if (batching == true)
        {
            for (auto& meshArray : m_LoadedMeshes)
            {
                std::string        materialName = meshArray.first;
                std::vector<Mesh*> mergedMeshes;
                for (int i = 0; i < meshArray.second.size(); i++)
                {
                    if (i == 0)
                    {
                        mergedMeshes.push_back(meshArray.second[i]);
                        continue;
                    }

                    Mesh*     lastMergeMesh       = mergedMeshes.back();
                    glm::vec3 lastMergeMeshCenter = lastMergeMesh->GetCenter();
                    glm::vec3 currentMeshCenter   = meshArray.second[i]->GetCenter();

                    if (glm::distance(lastMergeMeshCenter, currentMeshCenter) < 100.0f ||
                        lastMergeMesh->GetIndiceSize() < 100000)
                    {
                        Mesh::Merge(lastMergeMesh, meshArray.second[i]);
                        delete meshArray.second[i];
                    }
                    else
                    {
                        mergedMeshes.push_back(meshArray.second[i]);
                    }
                }
                meshArray.second = mergedMeshes;
            }
        }

        for (auto& meshArray : m_LoadedMeshes)
        {
            std::string materialName = meshArray.first;
            for (auto& mesh : meshArray.second)
            {
                mesh->CreateBuffer(layout);
            }
            m_LoadedMaterials.at(materialName)->BufferTextures();
        }

        for (auto& meshArray : m_LoadedMeshes)
        {
            std::string materialName = meshArray.first;
            for (auto& mesh : meshArray.second)
            {
                m_Meshes.push_back(mesh);
                m_Materials.push_back(m_LoadedMaterials.at(materialName));
            }
        }
    }

    StaticMesh::~StaticMesh()
    {
        for (auto& meshArray : m_LoadedMeshes)
        {
            for (auto& mesh : meshArray.second)
            {
                if (mesh != nullptr)
                {
                    delete mesh;
                }
            }
        }

        for (auto& material : m_LoadedMaterials)
        {
            if (material.second != nullptr)
            {
                delete material.second;
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
        Log::Info(fmt::format("Mesh Name: {0}", amesh->mName.C_Str()));
        Log::Info(fmt::format("Mesh NumVertices: {0}", amesh->mNumVertices));
        Log::Info(fmt::format("Mesh NumFaces: {0}", amesh->mNumFaces));

        Mesh* mesh = new Mesh();

        for (unsigned int i = 0; i < amesh->mNumVertices; i++)
        {
            Mesh::Vertex vertex;

            vertex.position = glm::vec3(amesh->mVertices[i].x, amesh->mVertices[i].y, amesh->mVertices[i].z);
            vertex.normal   = glm::vec3(amesh->mNormals[i].x, amesh->mNormals[i].y, amesh->mNormals[i].z);

            if (amesh->mTextureCoords[0])
            {
                vertex.texcoord  = glm::vec2(amesh->mTextureCoords[0][i].x, amesh->mTextureCoords[0][i].y);
                vertex.tangent   = glm::vec3(amesh->mTangents[i].x, amesh->mTangents[i].y, amesh->mTangents[i].z);
                vertex.bitangent = glm::vec3(amesh->mBitangents[i].x, amesh->mBitangents[i].y, amesh->mBitangents[i].z);
            }
            else
            {
                vertex.texcoord  = glm::vec2(0.0f, 0.0f);
                vertex.tangent   = glm::vec3(0.0f, 0.0f, 0.0f);
                vertex.bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            mesh->AddVertex(vertex);
        }

        for (unsigned int i = 0; i < amesh->mNumFaces; i++)
        {
            aiFace face = amesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                mesh->AddIndex(face.mIndices[j]);
            }
        }

        MBasicPbr*  material     = nullptr;
        std::string materialName = "_Default_Material_";

        if (amesh->mMaterialIndex > 0)
        {
            aiMaterial* mat = scene->mMaterials[amesh->mMaterialIndex];
            materialName    = mat->GetName().C_Str();
            Log::Info(fmt::format("Material Name: {0}", materialName));

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
                float roughness = glm::clamp(1.0f - shininess / 150.0f, 0.05f, 0.97f);
                material->SetRoughness(roughness);

                aiColor3D emissive;
                mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
                if (emissive.r > 0.0f || emissive.g > 0.0f || emissive.b > 0.0f)
                {
                    material->SetEnableEmissive(true);
                    material->SetEmissive(glm::vec3(emissive.r, emissive.g, emissive.b));
                    material->SetEmissiveIntensity(10.0f);
                }

                aiColor3D opacity;
                mat->Get(AI_MATKEY_COLOR_TRANSPARENT, opacity);
                material->SetOpacity(glm::vec3(opacity.r, opacity.g, opacity.b));

                Log::Info(fmt::format("Material Diffuse: {0}, {1}, {2}", diffuse.r, diffuse.g, diffuse.b));
                Log::Info(fmt::format("Material Specular: {0}, {1}, {2}", specular.r, specular.g, specular.b));
                Log::Info(fmt::format("Material Shininess: {0}", shininess));
                Log::Info(fmt::format("Material Roughness: {0}", roughness));
                Log::Info(fmt::format("Material Emissive: {0}, {1}, {2}", emissive.r, emissive.g, emissive.b));
                Log::Info(fmt::format("Material Opacity: {0}, {1}, {2}", opacity.r, opacity.g, opacity.b));

                aiString diffuseTexturePath;
                if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath) == AI_SUCCESS)
                {
                    std::string texturePath = m_Directory + "/" + diffuseTexturePath.C_Str();
                    Log::Info(fmt::format("Material Diffuse Texture Path: {0}", texturePath));
                    material->LoadDiffuseMap(texturePath);
                }

                aiString normalTexturePath;
                if (mat->GetTexture(aiTextureType_HEIGHT, 0, &normalTexturePath) == AI_SUCCESS)
                {
                    std::string texturePath = m_Directory + "/" + normalTexturePath.C_Str();
                    Log::Info(fmt::format("Material Normal Texture Path: {0}", texturePath));
                    // material->LoadNormalMap(texturePath);
                }

                aiString specularTexturePath;
                if (mat->GetTexture(aiTextureType_SPECULAR, 0, &specularTexturePath) == AI_SUCCESS)
                {
                    std::string texturePath = m_Directory + "/" + specularTexturePath.C_Str();
                    Log::Info(fmt::format("Material Specular Texture Path: {0}", texturePath));
                    material->LoadSpecularMap(texturePath);
                }

                aiString opacityTexturePath;
                if (mat->GetTexture(aiTextureType_OPACITY, 0, &opacityTexturePath) == AI_SUCCESS)
                {
                    std::string texturePath = m_Directory + "/" + opacityTexturePath.C_Str();
                    Log::Info(fmt::format("Material Opacity Texture Path: {0}", texturePath));
                    material->LoadOpacityMap(texturePath);
                }

                m_LoadedMaterials[materialName] = material;
            }
        }
        else
        {
            material = new MBasicPbr(materialName);
            material->SetWorkflow(0);

            m_LoadedMaterials[materialName] = material;
        }

        mesh->GenerateFrustumVolume();
        m_LoadedMeshes[materialName].push_back(mesh);
    }
} // namespace Engine