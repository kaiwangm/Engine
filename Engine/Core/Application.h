#pragma once

#include "Core.h"
#include "ApplicationEvent.h"
#include "LayerStack.h"
#include "Window.h"
#include "ImGuiLayer.h"

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

            inline Window& GetWindow() { return *m_Window; }

            static Application& Get() { return *s_Instance; }
            
        //Event Callbacks
        private:
            bool onWindowClose(WindowCloseEvent& e);

        private:
            Window* m_Window;
            ImGuiLayer* m_ImGuiLayer;
            LayerStack* m_LayerStack;
            bool m_Running = true;

            static Application* s_Instance;
    };

    Application* CreateApplication();
}