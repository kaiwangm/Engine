#pragma once

#include "Core.h"
#include "ApplicationEvent.h"
#include "LayerStack.h"
#include "Window.h"
#include "ImGuiLayer.h"
#include "Shader.h"

namespace Engine
{
    class ENGINE_API Application
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
            Window* m_Window;
            ImGuiLayer* m_ImGuiLayer;
            LayerStack* m_LayerStack;
            bool m_Running = true;

            static Application* s_Instance;
        
        // APP temp
        private:
            GLuint m_VertexArray, m_VertexBuffer, m_IndexBuffer;
            Shader* m_Shader;

    };

    Application* CreateApplication();
}