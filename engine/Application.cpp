#include "Application.h"
#include "Log.h"
#include <fmt/format.h>

#include "Events/ApplicationEvent.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    Application::Application()
    {
        Engine::Log::Init();
        ENGINE_CORE_TRACE("Engine Initialization");

        m_Window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application()
    {
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