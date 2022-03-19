#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "Core.h"
#include "Renderer.h"

namespace Engine {
struct Mesh {
    Ref<VertexArray> m_VertexArray;
    std::vector<Ref<Texture2D>> m_Textures;

    Mesh(float* vertices, uint32_t* indices, const uint32_t vertice_count,
         const uint32_t attribute_count, const uint32_t indice_count,
         const BufferLayout layout);

    void AddTexture(const Ref<Texture2D>& texture);
};

struct Model {
    Model();
    Model(const std::string& path);
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