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
            glm::vec3{0.246f, 0.250f, 8.100f});

        auto& board = Scene::CreateEntity(m_Scene, "board");
        m_Board = std::make_shared<Model>();

        board.AddComponent<StaticModelComponent>(m_Board);
        board.AddComponent<TransformComponent>();

        auto& gallery = Scene::CreateEntity(m_Scene, "gallery");
        // m_Gallery = std::make_shared<Model>("Assert/gallery/gallery.obj");

        // gallery.AddComponent<StaticModelComponent>(m_Gallery);
        // gallery.AddComponent<TransformComponent>();

        auto& animan = Scene::CreateEntity(m_Scene, "animan");
        m_Animan = std::make_shared<AnimatedModel>("Assert/animan/model.dae");

        animan.AddComponent<AnimatedModelComponent>(m_Animan);
        animan.AddComponent<TransformComponent>(
            glm::vec3{-0.010f, -1.980f, 1.340f},
            glm::vec3{-1.675f, -1.660f, 0.000f},
            glm::vec3{0.500f, 0.500f, 0.500f});
    }

    void OnAttach() override {}

    void OnDetach() override {}

    void OnUpdate() override {
        m_Camera->m_IsWindowFocused = m_IsWindowFocused;

        m_Scene->OnUpdateRuntime(m_LayerUpdateMeta.m_timeStep);
    }

    void OnImGuiRender() override {
        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        m_Scene->OnUpdateRuntimeGui(timeStep, nowTime);

        Gui::ShowImNodesDemoWindow();
        ImGui::ShowDemoWindow();

        bool is_color_focused = false;
        Gui::ShowViewport("ViewPort :: Color", m_Scene->m_FrameRenderBuffer,
                          is_color_focused);

        bool is_normal_focused = false;
        Gui::ShowViewport("ViewPort :: Normal",
                          m_Scene->m_FrameRenderBuffer_normal,
                          is_normal_focused);

        m_IsWindowFocused = is_color_focused | is_normal_focused;

        ImGui::ShowExampleAppLog(NULL);
    }

    void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

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
