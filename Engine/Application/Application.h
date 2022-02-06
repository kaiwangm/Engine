#pragma once

#include "Core.h"
#include "ApplicationEvent.h"
#include "LayerStack.h"
#include "Window.h"

namespace Engine
{
    class ENGINE_API Application
    {   
        public:
            Application();
            virtual ~Application();

            void Run();

            void onEvent(Event& e);

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* layer);

            inline Window& GetWinodw() { return *m_Window; }
            
        //Event Callbacks
        private:
            bool onWindowClose(WindowCloseEvent& e);

        private:
            Window* m_Window;
            LayerStack* m_LayerStack;
            bool m_Running = true;
    };

    Application* CreateApplication();
}