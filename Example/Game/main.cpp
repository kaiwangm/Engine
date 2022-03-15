#define ENGINE_USE_ENTRYPOINT
#include <Engine.h>

namespace Engine {
class ExampleLayer : public Layer {
   public:
    ExampleLayer() : Layer("Example"), m_IsWindowFocused(false) {
        m_Scene = std::make_shared<Scene>();

        // Set Shader
        m_Scene->LoadShader("TextureShader", "Assert/vertex.glsl",
                            "Assert/fragment.glsl", "Path");
        m_Scene->LoadShader("TextureShader_normal", "Assert/vertex_normal.glsl",
                            "Assert/fragment_normal.glsl", "Path");

        auto& board = Scene::CreateEntity(m_Scene, "board");
        m_Board = std::make_shared<Model>();

        board.AddComponent<StaticModelComponent>(m_Board);
        board.AddComponent<TransformComponent>();

        auto& bunny = Scene::CreateEntity(m_Scene, "bunny");
        m_Bunny = std::make_shared<Model>("Assert/bunny.obj");

        bunny.AddComponent<StaticModelComponent>(m_Bunny);
        bunny.AddComponent<TransformComponent>();
    }

    void OnAttach() override {}

    void OnDetach() override {}

    void OnUpdate() override {
        if (m_IsWindowFocused == true)
            m_Scene->m_Camera->OnUpdate(m_LayerUpdateMeta.m_timeStep);

        m_Scene->OnUpdateRuntime(m_LayerUpdateMeta.m_timeStep);
    }

    void OnImGuiRender() override {
        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        m_Scene->OnUpdateRuntimeGui(timeStep, nowTime);

        // Gui::ColorEdit4("backGroundColor", backGroundColor);
        //  ImGui::SliderFloat("transSpeed", &transSpeed, 0.0f, 6.0f);
        //  ImGui::SliderFloat("rotSpeed", &rotSpeed, 0.0f, 6.0f);

        Gui::Begin("Camera");
        Gui::SliderFloat3("m_Camera_Position", m_Scene->m_Camera->GetPosition(),
                          -10.0f, 10.0f);
        Gui::SliderFloat("m_Camera_Rotation", m_Scene->m_Camera->GetRotation(),
                         -180.0f, 180.0f);

        Gui::End();

        Gui::ShowImNodesDemoWindow();
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
            m_Scene->m_FrameRenderBuffer->SetViewPort((uint32_t)wsize.x,
                                                      (uint32_t)wsize.y);
            ImGui::Image(m_Scene->m_FrameRenderBuffer->GetTextureID(), wsize,
                         ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::Begin("ViewPort :: Normal");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("Render");
            // Get the size of the child (i.e. the whole draw size of the
            // windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V
            // from the UV.
            m_Scene->m_FrameRenderBuffer_normal->SetViewPort((uint32_t)wsize.x,
                                                             (uint32_t)wsize.y);
            ImGui::Image(m_Scene->m_FrameRenderBuffer_normal->GetTextureID(),
                         wsize, ImVec2(0, 1), ImVec2(1, 0));

            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::ShowExampleAppLog(NULL);
    }

    void OnEvent(Event& event) override { m_Scene->m_Camera->OnEvent(event); }

   private:
    Ref<Scene> m_Scene;

    Ref<Model> m_Board;
    Ref<Model> m_Bunny;

    bool m_IsWindowFocused;
};

class Sandbox : public Application {
   public:
    Sandbox() : Application("Sandbox", 2400, 1500) {
        Log::Trace("Sandbox Initialization.");
        PushLayer(std::make_shared<DockSpaceLayer>(m_Running));
        PushLayer(std::make_shared<ExampleLayer>());
        Log::Trace("Sandbox Initialization Success.");
    }

    ~Sandbox() { Log::Trace("Sandbox Shutdown."); }
};

Scope<Application> CreateApplication() { return std::make_unique<Sandbox>(); }
}  // namespace Engine
