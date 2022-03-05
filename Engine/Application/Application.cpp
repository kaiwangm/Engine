#include "Application.h"

#include "Input.h"
#include "Renderer.h"

namespace Engine {
Application* Application::s_Instance = nullptr;

Application::Application(std::string appName, uint32_t windowWidth,
                         uint32_t windowHeight)
    : m_Minimized(false), m_Running(true) {
    Engine::Log::Init();
    ENGINE_CORE_TRACE("Engine Initialization.");

    s_Instance = this;
    m_Window = Window::Create({appName, windowWidth, windowHeight});
    m_Window->SetEventCallback(BIND_EVENT(Application::OnEvent));
    m_Window->SetVSync(true);

    Renderer::Init();

    m_ImGuiLayer = ImGuiLayer::Create("ImGuiLayer");
    PushOverlay(m_ImGuiLayer);

    ENGINE_CORE_TRACE("Engine Initialization Success.");
}

Application::~Application() { ENGINE_CORE_TRACE("Engine Shutdown."); }

void Application::PushLayer(Ref<Layer> layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Ref<Layer> layer) {
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::Run() {
    while (m_Running) {
        // auto[x, y] = Input::GetMousePostion();
        // ENGINE_CORE_INFO("{0}, {1}", x, y);

        const LayerUpdateMeta updateMeta(m_Timer.GetTimeStep(),
                                         m_Timer.GetSeconds());

        for (const auto& layer : m_LayerStack) {
            layer->SetLayerUpdateMeta(updateMeta);
        }

        // if (m_Minimized == false) {
        for (const auto& layer : m_LayerStack) {
            layer->OnUpdate();
        }
        //}

        m_ImGuiLayer->Begin();
        for (const auto& layer : m_LayerStack) {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(
        BIND_EVENT(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(
        BIND_EVENT(Application::OnWindowResizeEvent));

    // ENGINE_CORE_INFO("Application <-- Event::{}", e);

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        if (event.m_Handled) break;
        (*it)->OnEvent(event);
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    m_Running = false;
    return true;
}

bool Application::OnWindowResizeEvent(WindowResizeEvent& event) {
    if (event.GetWidth() == 0 || event.GetHeight() == 0) {
        m_Minimized = true;
        return false;
    }
    m_Minimized = false;
    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

    return false;
}
}  // namespace Engine