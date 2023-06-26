#include "OctreeObject.h"
#include "ply_io.h"

namespace Engine
{
    OctreeObject::OctreeObject(const std::string& path, uint32_t maxLevel)
    {
        auto [coords, feats] = load_ply(path);

        m_HashOctree = std::make_shared<HashOctree<std::array<uint32_t, 3>>>(coords, feats, maxLevel);
    }

    void OctreeObject::CacheGL()
    {
        for (int i = 0; i <= m_HashOctree->GetMaxLevel(); ++i)
        {
            Ref<VertexArray> vertexArray;

            // ------------ OpenGL Box -------- //

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
                {0, ShaderDataType::Float3, "a_Position"}, {1, ShaderDataType::Float2, "a_TexCoord"},
                //{2, ShaderDataType::Float3, "a_Offset"},
                //{3, ShaderDataType::Float4, "a_Color"},
            });

            vertexArray = VertexArray::Create();
            vertexArray->AddVertexBuffer(box_VertexBuffer, false);

            // ------------ OpenGL Octree -------- //

            std::vector<float> points;
            auto& [coords, feats] = m_HashOctree->GetLevelNodes(i);
            float scale           = (float)m_HashOctree->GetScale(i);
            for (int idx = 0; idx < coords.size(); ++idx)
            {
                points.push_back(float(coords[idx][0]) * scale + scale / 2.0f);
                points.push_back(float(coords[idx][1]) * scale + scale / 2.0f);
                points.push_back(float(coords[idx][2]) * scale + scale / 2.0f);
                points.push_back(float(feats[idx][0]) / 255.0);
                points.push_back(float(feats[idx][1]) / 255.0);
                points.push_back(float(feats[idx][2]) / 255.0);
                // points.push_back((float)idx / (coords.size()));
                // points.push_back((float)idx / (coords.size()));
                // points.push_back((float)idx / (coords.size()));
                points.push_back(1.0f);
            }

            // points = std::vector<float>{0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
            // 0, 1};

            // Log::Core_Trace("{0}", points.size() / 7);

            Ref<VertexBuffer> octree_VertexBuffer =
                VertexBuffer::Create(&points[0], sizeof(float) * points.size(), points.size() / 7.0);
            octree_VertexBuffer->SetLayout({
                //{0, ShaderDataType::Float3, "a_Position"},
                //{1, ShaderDataType::Float2, "a_TexCoord"},
                {2, ShaderDataType::Float3, "a_Offset"},
                {3, ShaderDataType::Float4, "a_Color"},
            });

            vertexArray->AddVertexBuffer(octree_VertexBuffer, true);

            m_Octree_VertexArrays.push_back(vertexArray);
        }

        auto [coords_10, feats_10] = m_HashOctree->GetLevelNodes(10);
        for (uint32_t idx = 0; idx < feats_10.size(); ++idx)
        {
            bitmask_t coord[2] {0, 0};
            // int x = idx / 1280;
            // int y = idx % 1280;
            // d2xy(1280, idx, &x, &y);
            hilbert_i2c(2, 12, (bitmask_t)idx, coord);

            // texture[x][y][0] = (float)idx / (1280*1280) * 255;
            // texture[x][y][1] = (float)idx / (1280*1280) * 255;
            // texture[x][y][2] = (float)idx / (1280*1280) * 255;

            int x = coord[0];
            int y = coord[1];

            texture[x][y][0] = feats_10[idx][0];
            texture[x][y][1] = feats_10[idx][1];
            texture[x][y][2] = feats_10[idx][2];
        }

        m_Octree_Texture = Texture2D::Create();
        m_Octree_Texture->Load(texture, 1280, 1280);
    }

    void OctreeObject::RenderOctree(uint32_t level, const Ref<Shader> shader)
    {
        auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        Renderer::SetShaderUniform(shader, "u_Color", glm::vec4(0.3f, 0.0f, 0.0f, 1.0f));

        float scale = (1 << (m_HashOctree->GetMaxLevel() - level));
        Renderer::SetShaderUniform(shader, "u_Scale", scale);

        Renderer::DrawArray(m_Octree_VertexArrays[level], shader, transform);
        return;
    }

    void OctreeObject::RenderOctreeGui()
    {
        ImGui::Begin("Octree");
        ImGui::Text("NumMaxLevel: %d", m_HashOctree->GetMaxLevel());
        ImGui::Text("NumNodes: %d", m_HashOctree->GetNumNodes());
        ImGui::Text("NumLeafs: %d", m_HashOctree->GetNumLeafs());

        for (int i = 0; i <= m_HashOctree->GetMaxLevel(); ++i)
        {
            ImGui::Text("Laver %d has %d Nodes", i, (int)m_HashOctree->GetLevelNumNodes(i));
        }
        ImGui::End();

        ImGui::Begin("ViewPort :: Texture");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("Render");
            // m_IsWindowFocused = ImGui::IsWindowFocused();
            //  Log::Trace("Focused: {0}", ImGui::IsWindowFocused());
            //   Get the size of the child (i.e. the whole draw size of the
            //   windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            //  Because I use the texture from OpenGL, I need to invert the V
            //  from the UV.
            // m_FrameRenderBuffer->SetViewPort((uint32_t)1024, (uint32_t)1024);
            ImGui::Image(m_Octree_Texture->GetTextureID(), wsize);
            ImGui::EndChild();
        }
        ImGui::End();
        return;
    }
} // namespace Engine