#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Core.h"
#include "Renderer.h"

#include "../DataUtil/Mesh.h"

namespace Engine {
struct StaticMesh {
    StaticMesh();
    StaticMesh(const std::string& path);
    std::string m_Directory;

    std::vector<Mesh> m_Meshes;
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat,
                                                     aiTextureType type,
                                                     std::string typeName);
    std::vector<Ref<Texture2D>> m_Textures;
};
}  // namespace Engine