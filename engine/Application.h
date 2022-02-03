#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core.h"
#include "Window.h"
#include "Log.h"
#include "Events/ApplicationEvent.h"


namespace Engine
{
    class ENGINE_API Application
    {
        public:
            Application();
            virtual ~Application();

            void Run();
        private:
            std::unique_ptr<Window> m_Window;
            bool m_Running = true;
    };

    Application* CreateApplication();
}