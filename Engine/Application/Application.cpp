#include "Application.h"

namespace Engine
{

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

    Application::Application()
    {
        Engine::Log::Init();
        ENGINE_CORE_TRACE("Engine Initialization");

        m_Window = Window::Create();
        m_Window->SetEventCallback(BIND_EVENT(Application::onEvent));

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    Application::~Application()
    {
        delete m_Window;
        ENGINE_CORE_TRACE("Engine Shutdown");
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }

    void Application::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::onWindowClose));

        ENGINE_CORE_INFO(e);
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}