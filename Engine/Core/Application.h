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
            std::shared_ptr<Shader> m_Shader;

            std::shared_ptr<VertexArray> m_Triangle_VertexArray;
            std::shared_ptr<VertexArray> m_Square_VertexArray;
    };

    Application* CreateApplication();
}