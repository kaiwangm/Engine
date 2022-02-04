#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
    #include <windows.h>
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#endif

#include "Core.h"
#include "Window.h"
#include "Log.h"
#include "ApplicationEvent.h"

namespace Engine
{
    class ENGINE_API Application
    {   
        public:
            Application();
            virtual ~Application();

            void Run();

            //Event Callbacks
            void onEvent(Event& e);
            bool onWindowClose(WindowCloseEvent& e);
            
        private:
            Window* m_Window;
            bool m_Running = true;
    };

    Application* CreateApplication();
}