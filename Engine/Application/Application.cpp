#include "Application.h"

#include "ImGuiLayer.h"
#include "Input.h"
#include "Log.h"

namespace Engine {
Application* Application::s_Instance = nullptr;

Application::Application() {
    Engine::Log::Init();
    ENGINE_CORE_TRACE("Engine Initialization.");

    s_Instance = this;
    m_Window = Window::Create({"Game Engine", 1600, 900});
    m_Window->SetEventCallback(BIND_EVENT(Application::onEvent));
    m_Window->SetVSync(true);

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

        for (const auto& layer : m_LayerStack) {
            layer->OnUpdate();
        }

        m_ImGuiLayer->Begin();
        for (const auto& layer : m_LayerStack) {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }
}

void Application::onEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(
        BIND_EVENT(Application::onWindowClose));

    // ENGINE_CORE_INFO("Application <-- Event::{}", e);

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        if (e.m_Handled) break;
        (*it)->OnEvent(e);
    }
}

bool Application::onWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return true;
}
}  // namespace Engine