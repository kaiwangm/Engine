#define ENGINE_USE_ENTRYPOINT
#include <Engine.h>

namespace Engine {
class EditorLayer : public Layer {
   public:
    EditorLayer() : Layer("EditorLayer"), m_IsWindowFocused(false) {
        m_World = std::make_shared<UWorld>();
        m_World->AddActor<AActor>("Actor");

        m_World->LoadShader("TextureShader", "Assert/Shader/vertex.glsl",
                            "Assert/Shader/fragment.glsl", "Path");
        m_World->LoadShader("TextureShader_normal",
                            "Assert/Shader/vertex_normal.glsl",
                            "Assert/Shader/fragment_normal.glsl", "Path");
        m_World->LoadShader("Animated", "Assert/Shader/vertex_animated.glsl",
                            "Assert/Shader/fragment_animated.glsl", "Path");

        m_Camera = std::make_shared<PerspectiveCamera>(45.0f, 1.778f, 0.1f,
                                                       3000.0f * 8);
        auto camera = m_World->AddActor<ACamera>("Camera", m_Camera);
        camera.GetTransformComponent().SetPosition(
            glm::vec3{-1.774f, 4.034f, 9.425f});
        camera.GetTransformComponent().SetRotation(
            glm::vec3{-0.153f, -6.606f, 0.000f});
        camera.GetTransformComponent().SetScale(
            glm::vec3{1.000f, 1.000f, 1.000f});

        auto board = m_World->AddActor<AStaticMesh>("board");
        board.GetTransformComponent().SetPosition(
            glm::vec3{-2.350f, 2.165f, 0.000f});

        auto gallery = m_World->AddActor<AStaticMesh>(
            "gallery", "Assert/Object/gallery/gallery.obj");

        auto animan = m_World->AddActor<AAnimatedMesh>(
            "animan", "Assert/Object/animan/model.dae");

        animan.GetTransformComponent().SetPosition(
            glm::vec3{1.655f, 0.685f, 0.120f});
        animan.GetTransformComponent().SetRotation(
            glm::vec3{-1.330f, 0.000f, 0.000f});
        animan.GetTransformComponent().SetScale(
            glm::vec3{0.300f, 0.300f, 0.300f});
    }

    void OnAttach() override {}

    void OnDetach() override {}

    void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

    void TickLogic() override {
        m_World->TickLogic(m_LayerUpdateMeta.m_timeStep,
                           m_LayerUpdateMeta.m_nowTime, m_IsWindowFocused);
    }

    void TickRender() override {
        m_World->TickRender(m_LayerUpdateMeta.m_timeStep,
                            m_LayerUpdateMeta.m_nowTime);

        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        Gui::ShowImNodesDemoWindow();
        ImGui::ShowDemoWindow();

        bool is_color_focused = false;
        Gui::ShowViewport("ViewPort :: Color", m_World->m_FrameRenderBuffer,
                          true, is_color_focused);

        bool is_normal_focused = false;
        Gui::ShowViewport("ViewPort :: Normal",
                          m_World->m_FrameRenderBuffer_normal, false,
                          is_normal_focused);

        m_IsWindowFocused = is_color_focused | is_normal_focused;

        ImGui::ShowExampleAppLog(NULL);
    }

   private:
    Ref<Camera> m_Camera;
    Ref<UWorld> m_World;

    bool m_IsWindowFocused;
};

class EngineEditor : public Application {
   public:
    EngineEditor() : Application("EngineEditor", 2700, 1500) {
        Log::Info("EngineEditor Initialization.");
        PushLayer(std::make_shared<DockSpaceLayer>(m_Running));
        PushLayer(std::make_shared<EditorLayer>());
        Log::Trace("EngineEditor Initialization Success.");
    }

    ~EngineEditor() { Log::Trace("EngineEditor Shutdown."); }
};

Scope<Application> CreateApplication() {
    return std::make_unique<EngineEditor>();
}
}  // namespace Engine
