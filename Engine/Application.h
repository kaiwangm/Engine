#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
    #include <windows.h>
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#endif

#include "Core.h"
#include "Window/Window.h"
#include "Log/Log.h"
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
            Window* m_Window;
            bool m_Running = true;
    };

    Application* CreateApplication();
}