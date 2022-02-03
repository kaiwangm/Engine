#include "Application.h"

namespace Engine
{
    Application::Application()
    {
        Engine::Log::Init();
        ENGINE_CORE_TRACE("Engine Initialization");

        m_Window = Window::Create();
    }

    Application::~Application()
    {
        delete m_Window;
        ENGINE_CORE_TRACE("Engine Shutdown");
    }

    void Application::Run()
    {
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        
        while (m_Running)
        {
            glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
}