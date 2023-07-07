#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Renderer/Renderer.h>

namespace Engine
{
    struct PointCloud
    {
        PointCloud();
        PointCloud(const std::string& filepath);
        std::string m_FilePath;

        Ref<VertexArray>                    m_VertexArray;
        std::vector<std::array<float, 3>>   m_vertices;
        std::vector<std::array<uint8_t, 3>> m_colors;

        int m_NumPoints;
    };
} // namespace Engine