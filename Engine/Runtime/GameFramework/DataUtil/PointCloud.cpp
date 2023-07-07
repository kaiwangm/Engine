#include <Engine/Runtime/GameFramework/DataUtil/PointCloud.h>

#include "happly.h"

namespace Engine
{
    PointCloud::PointCloud() { m_VertexArray = VertexArray::Create(); }

    PointCloud::PointCloud(const std::string& filepath) : m_FilePath(filepath)
    {
        happly::PLYData plyIn(m_FilePath);
        // plyIn.write(name, happly::DataFormat::Binary);
        auto x = plyIn.getElement("vertex").getProperty<float>("x");
        auto y = plyIn.getElement("vertex").getProperty<float>("y");
        auto z = plyIn.getElement("vertex").getProperty<float>("z");
        auto r = plyIn.getElement("vertex").getProperty<unsigned char>("red");
        auto g = plyIn.getElement("vertex").getProperty<unsigned char>("green");
        auto b = plyIn.getElement("vertex").getProperty<unsigned char>("blue");

        m_NumPoints = x.size();

        m_vertices = std::vector<std::array<float, 3>>(m_NumPoints);
        m_colors   = std::vector<std::array<uint8_t, 3>>(m_NumPoints);

        for (int i = 0; i < m_NumPoints; ++i)
        {
            m_vertices[i][0] = x[i];
            m_vertices[i][1] = y[i];
            m_vertices[i][2] = z[i];
            m_colors[i][0]   = r[i];
            m_colors[i][1]   = g[i];
            m_colors[i][2]   = b[i];
        }

        float box_vertices[36 * 5] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //

            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //

            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //
            -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, //
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
            0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, //
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
            0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, //
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //

            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
            -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, //
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
        };

        Ref<VertexBuffer> box_VertexBuffer = VertexBuffer::Create(box_vertices, sizeof(box_vertices), 36);
        box_VertexBuffer->SetLayout({
            {0, ShaderDataType::Float3, "a_Position"},
            {1, ShaderDataType::Float2, "a_TexCoord"},
        });

        m_VertexArray = VertexArray::Create();
        m_VertexArray->AddVertexBuffer(box_VertexBuffer, false);

        std::vector<float> points;
        for (int idx = 0; idx < x.size(); ++idx)
        {
            points.push_back(float(m_vertices[idx][0]));
            points.push_back(float(m_vertices[idx][1]));
            points.push_back(float(m_vertices[idx][2]));
            points.push_back(float(m_colors[idx][0]) / 255.0);
            points.push_back(float(m_colors[idx][1]) / 255.0);
            points.push_back(float(m_colors[idx][2]) / 255.0);
            // points.push_back((float)idx / (coords.size()));
            // points.push_back((float)idx / (coords.size()));
            // points.push_back((float)idx / (coords.size()));
            points.push_back(1.0f);
        }

        Ref<VertexBuffer> octree_VertexBuffer =
            VertexBuffer::Create(&points[0], sizeof(float) * points.size(), points.size() / 7);
        octree_VertexBuffer->SetLayout({
            //{0, ShaderDataType::Float3, "a_Position"},
            //{1, ShaderDataType::Float2, "a_TexCoord"},
            {2, ShaderDataType::Float3, "a_Offset"},
            {3, ShaderDataType::Float4, "a_Color"},
        });

        m_VertexArray->AddVertexBuffer(octree_VertexBuffer, true);
    }
} // namespace Engine