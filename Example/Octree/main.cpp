#define NOMINMAX

#include <Engine.h>

#include <future>
#include <thread>

#include "Octree/Octree.h"
#include "ply_io.h"

namespace Engine {

class ExampleLayer : public Layer {
   public:
    ExampleLayer() : Layer("Example") {
        // ------------ Octree -------- //
        auto octree = std::make_shared<Octree>(
            load_ply("Assert/longdress_vox10_1300.ply"), 10);
        m_Octree = octree;
        Log::Trace("Octree built successfully.");

        // ------------ Game -------- //
        Log::Trace("Init game meta.");

        m_Camera = std::make_shared<PerspectiveCamera>(45.0f, 1.778f, 0.1f,
                                                       3000.0f * 8);
        m_Camera->SetPosition({224.0f, 453.0f, 729.0f});
        m_Camera->SetCameraTranslationSpeed(300.0f);
        m_Camera->SetCameraRotationSpeed(10.0f);

        for (int i = 0; i <= 10; ++i) {
            Ref<VertexArray> vertexArray;

            // ------------ OpenGL Box -------- //

            float box_vertices[36 * 5] = {
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
                0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  //
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
                -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //

                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
                0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
                -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  //
                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //

                -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //
                -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  //
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
                -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
                0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
                0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
                0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  //
                0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
                0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //

                -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
                -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  //
                -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
            };

            Ref<VertexBuffer> box_VertexBuffer =
                VertexBuffer::Create(box_vertices, sizeof(box_vertices), 36);
            box_VertexBuffer->SetLayout({
                {0, ShaderDataType::Float3, "a_Position"},
                {1, ShaderDataType::Float2, "a_TexCoord"},
                //{2, ShaderDataType::Float3, "a_Offset"},
                //{3, ShaderDataType::Float4, "a_Color"},
            });

            vertexArray = VertexArray::Create();
            vertexArray->AddVertexBuffer(box_VertexBuffer, false);

            // ------------ OpenGL Octree -------- //

            std::vector<float> points;
            for (const Ref<OctreeNode>& p : octree->GetLevelNodes(i)) {
                points.push_back(float(p->GetX()));
                points.push_back(float(p->GetY()));
                points.push_back(float(p->GetZ()));
                points.push_back(float(p->GetR()) / 255.0);
                points.push_back(float(p->GetG()) / 255.0);
                points.push_back(float(p->GetB()) / 255.0);
                points.push_back(1.0f);
            }

            // points = std::vector<float>{0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
            // 0, 1};

            Ref<VertexBuffer> octree_VertexBuffer = VertexBuffer::Create(
                &points[0], sizeof(float) * points.size(), points.size() / 7.0);
            octree_VertexBuffer->SetLayout({
                //{0, ShaderDataType::Float3, "a_Position"},
                //{1, ShaderDataType::Float2, "a_TexCoord"},
                {2, ShaderDataType::Float3, "a_Offset"},
                {3, ShaderDataType::Float4, "a_Color"},
            });

            vertexArray->AddVertexBuffer(octree_VertexBuffer, true);

            m_Octree_VertexArrays.push_back(vertexArray);
        }

        // ---------------Shader--------------- //
        m_ShaderLibrary.Load("OctreeShader", "Assert/octree_vertex.glsl",
                             "Assert/octree_fragment.glsl", "Path");

        // Set FrameRenderBuffer
        m_FrameRenderBuffer = FrameRenderBuffer::Create();
    }

    void OnAttach() override {}

    void OnDetach() override {}

    void OnUpdate() override {
        if (m_IsWindowFocused == true)
            m_Camera->OnUpdate(m_LayerUpdateMeta.m_timeStep);

        // Render
        Renderer::BeginScene(m_Camera, m_FrameRenderBuffer);

        RenderCommand::SetClearColor(backGroundColor);
        RenderCommand::Clear();

        this->RenderOctree(m_Octree);

        Renderer::EndScene(m_FrameRenderBuffer);
    }

    void OnImGuiRender() override {
        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        Gui::Begin("Scence Collection");
        Gui::Text("Time Step: {0}", timeStep);
        Gui::Text("Now Time: {0}", nowTime);

        static std::vector<float> arr(600, 0.0);
        arr.push_back(timeStep);
        arr.erase(arr.begin());
        ImGui::PlotLines("Frame Times", &arr[0], 600);

        Gui::ColorEdit4("backGroundColor", backGroundColor);
        // ImGui::SliderFloat("transSpeed", &transSpeed, 0.0f, 6.0f);
        // ImGui::SliderFloat("rotSpeed", &rotSpeed, 0.0f, 6.0f);

        Gui::SliderFloat3("m_Camera_Position", m_Camera->GetPosition(), -10.0f,
                          10.0f);
        Gui::SliderFloat("m_Camera_Rotation", m_Camera->GetRotation(), -180.0f,
                         180.0f);

        uint32_t nowLevel_min = 0;
        uint32_t nowLevel_max = 10;
        ImGui::SliderScalar("nowLevel", ImGuiDataType_U32, &nowLevel,
                            &nowLevel_min, &nowLevel_max);

        Gui::End();

        ImGui::ShowDemoWindow();

        ImGui::Begin("ViewPort :: Color");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("Render");
            m_IsWindowFocused = ImGui::IsWindowFocused();
            // Log::Trace("Focused: {0}", ImGui::IsWindowFocused());
            //  Get the size of the child (i.e. the whole draw size of the
            //  windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V
            // from the UV.
            m_FrameRenderBuffer->SetViewPort((uint32_t)wsize.x,
                                             (uint32_t)wsize.y);
            ImGui::Image(m_FrameRenderBuffer->GetTextureID(), wsize,
                         ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::ShowExampleAppLog(NULL);
        
        this->RenderOctreeGui(m_Octree);
    }

    void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

    void RenderOctree(const Ref<Octree>& octree) {
        if (octree == nullptr) {
            return;
        } else {
            auto transform =
                glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            auto m_Shader = m_ShaderLibrary.Get("OctreeShader");

            Renderer::SetShaderUniform(m_Shader, "u_Color",
                                       glm::vec4(0.3f, 0.0f, 0.0f, 1.0f));

            float scale = (1 << (octree->GetMaxLevel() - nowLevel));
            Renderer::SetShaderUniform(m_Shader, "u_Scale", scale);

            Renderer::DrawArray(m_Octree_VertexArrays[nowLevel], m_Shader,
                                transform);
            return;
        }
    }

    void RenderOctreeGui(const Ref<Octree>& octree) {
        if (octree == nullptr) {
            ImGui::Begin("Octree");
            ImGui::Text("Loading Octree.");
            ImGui::End();
            return;
        } else {
            // threadObj.join();
            ImGui::Begin("Octree");
            ImGui::Text("NumMaxLevel: %d", octree->GetMaxLevel());
            ImGui::Text("NumNodes: %d", octree->GetNumNodes());
            ImGui::Text("NumLeafs: %d", octree->GetNumLeafs());

            for (int i = 0; i <= octree->GetMaxLevel(); ++i) {
                ImGui::Text("Laver %d has %d Nodes", i,
                            (int)octree->GetLevelNumNodes(i));
            }
            ImGui::End();
            return;
        }
    }

   private:
    Ref<Camera> m_Camera;
    ShaderLibrary m_ShaderLibrary;

    Ref<Octree> m_Octree;
    std::vector<Ref<VertexArray> > m_Octree_VertexArrays;

    Ref<FrameRenderBuffer> m_FrameRenderBuffer;

   private:
    glm::vec4 backGroundColor{0.7f, 0.7f, 0.7f, 1.0f};
    bool m_IsWindowFocused;

    uint32_t nowLevel = 10;
};

class Sandbox : public Application {
   public:
    Sandbox() : Application("OctreeExample", 2500, 1800) {
        Log::Trace("Sandbox Initialization.");
        PushLayer(std::make_shared<DockSpaceLayer>(m_Running));
        PushLayer(std::make_shared<ExampleLayer>());
        Log::Trace("Sandbox Initialization Success.");
    }

    ~Sandbox() { Log::Trace("Sandbox Shutdown."); }
};

Scope<Application> CreateApplication() { return std::make_unique<Sandbox>(); }
}  // namespace Engine
