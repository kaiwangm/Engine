#include <Engine/Runtime/GameFramework/DataUtil/PointCloud.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <openvdb/openvdb.h>
#include <openvdb/tools/LevelSetSphere.h>

namespace Engine
{
    PointCloud::PointCloud() { m_VertexArray = VertexArray::Create(); }

    PointCloud::PointCloud(const std::string& filepath) : m_FilePath(filepath)
    {
        openvdb::initialize();
        openvdb::FloatGrid::Ptr      grid     = openvdb::FloatGrid::create();
        openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
        openvdb::Coord               xyz(1000, -200000000, 30000000);
        accessor.setValue(xyz, 1.0);

        Log::Info(fmt::format("Grid xyz = {}", accessor.getValue(xyz)));

        pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointcloud(new pcl::PointCloud<pcl::PointXYZRGB>);

        if (pcl::io::loadPLYFile<pcl::PointXYZRGB>(m_FilePath, *pointcloud) == -1)
        {
            Log::Error(fmt::format("Couldn't read point cloud file: {0}", m_FilePath));
            return;
        }

        int numPoints = pointcloud->points.size();
        m_NumPoints   = numPoints;

        m_vertices = std::vector<std::array<float, 3>>(numPoints);
        m_colors   = std::vector<std::array<uint8_t, 3>>(numPoints);

        for (int i = 0; i < numPoints; ++i)
        {
            m_vertices[i][0] = pointcloud->points[i].x;
            m_vertices[i][1] = pointcloud->points[i].y;
            m_vertices[i][2] = pointcloud->points[i].z;
            m_colors[i][0]   = pointcloud->points[i].r;
            m_colors[i][1]   = pointcloud->points[i].g;
            m_colors[i][2]   = pointcloud->points[i].b;
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
        for (int idx = 0; idx < numPoints; ++idx)
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