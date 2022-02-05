#include "Application.h"

#ifdef ENGINE_PLATFORM_WINDOWS
    #include <windows.h>
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#endif

#include "Log.h"
#include "ImGuiLayer.h"

namespace Engine
{
    Application::Application()
    {
        Engine::Log::Init();
        ENGINE_CORE_TRACE("Engine Initialization");

        m_Window = Window::Create();
        m_Window->SetEventCallback(BIND_EVENT(Application::onEvent));

        PushLayer(new ImGuiLayer(this));
    }

    Application::~Application()
    {
        delete m_Window;
        ENGINE_CORE_TRACE("Engine Shutdown");
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for(auto layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::onWindowClose));

        ENGINE_CORE_INFO("Application <-- Event::{}", e);

        for(auto it = m_LayerStack.end(); it!=m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.m_Handled)
                break;
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}