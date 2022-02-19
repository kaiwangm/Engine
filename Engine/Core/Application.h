#pragma once

#include "Core.h"
#include "ApplicationEvent.h"
#include "LayerStack.h"
#include "Window.h"
#include "ImGuiLayer.h"
#include "Shader.h"
#include "Buffer.h"

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
            std::unique_ptr<LayerStack> m_LayerStack;
            bool m_Running = true;

            static Application* s_Instance;
        
        // APP temp
        private:
            GLuint m_VertexArray;
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<VertexBuffer> m_VertexBuffer;
            std::unique_ptr<IndexBuffer> m_IndexBuffer;

    };

    Application* CreateApplication();
}