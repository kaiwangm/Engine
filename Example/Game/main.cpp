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
        m_Scene->LoadShader("Animated", "Assert/vertex_animated.glsl",
                            "Assert/fragment_animated.glsl", "Path");

        auto& camera = Scene::CreateEntity(m_Scene, "camera");
        m_Camera = std::make_shared<PerspectiveCamera>(45.0f, 1.778f, 0.1f,
                                                       3000.0f * 8);

        camera.AddComponent<CameraComponent>(m_Camera);
        camera.AddComponent<TransformComponent>(
            glm::vec3{-1.774f, 4.034f, 9.425f},
            glm::vec3{-0.153f, -6.606f, 0.000f},
            glm::vec3{1.000f, 1.000f, 1.000f});

        auto& board = Scene::CreateEntity(m_Scene, "board");
        m_Board = std::make_shared<Model>();

        board.AddComponent<StaticModelComponent>(m_Board);
        board.AddComponent<TransformComponent>(
            glm::vec3{-2.350f, 2.165f, 0.000f});

        auto& gallery = Scene::CreateEntity(m_Scene, "gallery");
        m_Gallery = std::make_shared<Model>("Assert/gallery/gallery.obj");

        gallery.AddComponent<StaticModelComponent>(m_Gallery);
        gallery.AddComponent<TransformComponent>();

        auto& animan = Scene::CreateEntity(m_Scene, "animan");
        m_Animan = std::make_shared<AnimatedModel>("Assert/animan/model.dae");

        animan.AddComponent<AnimatedModelComponent>(m_Animan);
        animan.AddComponent<TransformComponent>(
            glm::vec3{1.655f, 0.685f, 0.120f},
            glm::vec3{-1.330f, 0.000f, 0.000f},
            glm::vec3{0.300f, 0.300f, 0.300f});
    }

    void OnAttach() override {}

    void OnDetach() override {}

    void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

    void TickLogic() override {
        m_Scene->TickLogic(m_LayerUpdateMeta.m_timeStep,
                           m_LayerUpdateMeta.m_nowTime, m_IsWindowFocused);
    }

    void TickRender() override {
        m_Scene->TickRender(m_LayerUpdateMeta.m_timeStep,
                            m_LayerUpdateMeta.m_nowTime);

        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        Gui::ShowImNodesDemoWindow();
        ImGui::ShowDemoWindow();

        bool is_color_focused = false;
        Gui::ShowViewport("ViewPort :: Color", m_Scene->m_FrameRenderBuffer,
                          true, is_color_focused);

        bool is_normal_focused = false;
        Gui::ShowViewport("ViewPort :: Normal",
                          m_Scene->m_FrameRenderBuffer_normal, false,
                          is_normal_focused);

        m_IsWindowFocused = is_color_focused | is_normal_focused;

        ImGui::ShowExampleAppLog(NULL);
    }

   private:
    Ref<Scene> m_Scene;
    Ref<Camera> m_Camera;

    Ref<Model> m_Board;
    Ref<Model> m_Gallery;
    Ref<AnimatedModel> m_Animan;

    bool m_IsWindowFocused;
};

class Sandbox : public Application {
   public:
    Sandbox() : Application("Sandbox", 2700, 1500) {
        Log::Trace("Sandbox Initialization.");
        PushLayer(std::make_shared<DockSpaceLayer>(m_Running));
        PushLayer(std::make_shared<ExampleLayer>());
        Log::Trace("Sandbox Initialization Success.");
    }

    ~Sandbox() { Log::Trace("Sandbox Shutdown."); }
};

Scope<Application> CreateApplication() { return std::make_unique<Sandbox>(); }
}  // namespace Engine
