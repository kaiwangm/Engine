#pragma once

#include "Core.h"
#include "ApplicationEvent.h"
#include "LayerStack.h"
#include "Window.h"
#include "ImGuiLayer.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

namespace Engine
{
    class Application
    {   
        public:
            Application();
            virtual ~Application();

            void Run();
            virtual void ClientDraw() = 0;

            void onEvent(Event& e);

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* layer);

            inline Window& GetWindow() { return *m_Window; }

            static Application& Get() { return *s_Instance; }
            
        //Event Callbacks
        private:
            bool onWindowClose(WindowCloseEvent& e);

        private:
            Scope<Window> m_Window;
            Scope<LayerStack> m_LayerStack;
            bool m_Running = true;

            ImGuiLayer* m_ImGuiLayer;

            static Application* s_Instance;
    };

    Scope<Application> CreateApplication();
}