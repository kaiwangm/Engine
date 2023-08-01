#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/GameFramework/Material/MMaterial.h>
#include <Engine/Runtime/GameFramework/DataUtil/Mesh.h>

namespace Engine
{
    struct StaticMesh
    {
        StaticMesh();
        StaticMesh(const std::string& path);
        std::string m_Directory;

        std::vector<Mesh*>      m_Meshes;
        std::vector<MMaterial*> m_Materials;

        std::unordered_map<std::string, MMaterial*> m_LoadedMaterials;

        void processNode(aiNode* node, const aiScene* scene);
        void processMesh(aiMesh* mesh, const aiScene* scene);
    };
} // namespace Engine