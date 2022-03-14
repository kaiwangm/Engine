#define ENGINE_USE_ENTRYPOINT
#include <Engine.h>

//
#include <tbb/tbb.h>

#include "OctreeObject.h"
#include "ply_io.h"

namespace Engine {
class ExampleLayer : public Layer {
   public:
    ExampleLayer() : Layer("Example") {
        // std::string ply_name = "soldier_vox10_0835";
        // std::string ply_path = "Assert/" + ply_name + ".ply";

        /*
        for (int i = 0; i <= 330; ++i) {
            std::string ply_name = fmt::format("{:06d}", i);
            std::string ply_path = "Assert/Kitti/" + ply_name + ".ply";
            auto [coords, feats] = load_ply(ply_path);

            auto hashOctree =
                std::make_shared<HashOctree<std::array<uint32_t, 3>>>(
                    coords, feats, 10);
            xt::xarray<int> batch = hashOctree->GetContexBatch(16, 4);
            xt::dump_npy("out/" + ply_name + "_out.npy", batch);
        }
        */

        m_Octrees.resize(15);

        /*
        for (int i = 0; i < m_Octrees.size(); ++i) {
            std::string ply_name = fmt::format("{:06d}", i);
            std::string ply_path = "Assert/Kitti/" + ply_name + ".ply";

            m_Octrees[i] = std::make_shared<OctreeObject>(ply_path, 12);
            m_Octrees[i]->CacheGL();
        }
        */

        /*
        tbb::parallel_for(0, 19, [&](size_t i) {
            uint32_t idx = 1450 + i;

            std::cout << i << std::endl;
            m_Octrees[i] = std::make_shared<OctreeObject>(
                "Assert/redandblack/Ply/redandblack_vox10_" +
                    std::to_string(idx) + ".ply",
                10);
            std::cout << i << std::endl;
        });
        */

        
        for (int i = 0; i < m_Octrees.size(); ++i) {
            uint32_t idx = 1450 + i;
            std::cout << i << std::endl;

            m_Octrees[i] = std::make_shared<OctreeObject>(
                "Assert/redandblack/Ply/redandblack_vox10_" +
                    std::to_string(idx) + ".ply",
                10);
            m_Octrees[i]->CacheGL();
        }
        

        Log::Trace("Octree built successfully.");

        // ------------ Game -------- //
        Log::Trace("Init game meta.");

        m_Camera = std::make_shared<PerspectiveCamera>(45.0f, 1.778f, 0.1f,
                                                       3000.0f * 8);
        m_Camera->SetPosition({224.0f, 453.0f, 729.0f});
        m_Camera->SetCameraTranslationSpeed(300.0f);
        m_Camera->SetCameraRotationSpeed(10.0f);

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

        m_Octrees[nowFrame]->RenderOctree(nowLevel,
                                          m_ShaderLibrary.Get("OctreeShader"));

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
        uint32_t nowLevel_max = 12;
        ImGui::SliderScalar("nowLevel", ImGuiDataType_U32, &nowLevel,
                            &nowLevel_min, &nowLevel_max);

        uint32_t nowFrame_min = 0;
        uint32_t nowFrame_max = m_Octrees.size() - 1;
        ImGui::SliderScalar("nowFrame", ImGuiDataType_U32, &nowFrame,
                            &nowFrame_min, &nowFrame_max);

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

        m_Octrees[nowFrame]->RenderOctreeGui();
    }

    void OnEvent(Event &event) override { m_Camera->OnEvent(event); }

   private:
    Ref<Camera> m_Camera;
    ShaderLibrary m_ShaderLibrary;

    Ref<FrameRenderBuffer> m_FrameRenderBuffer;

    tbb::concurrent_vector<Ref<OctreeObject>> m_Octrees;

   private:
    glm::vec4 backGroundColor{0.7f, 0.7f, 0.7f, 1.0f};
    bool m_IsWindowFocused;

    uint32_t nowLevel = 10;
    uint32_t nowFrame = 0;
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
