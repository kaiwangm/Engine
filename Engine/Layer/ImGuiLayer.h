#pragma once

#include "Layer.h"
#include "Application.h"

#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Engine 
{
    class ImGuiLayer : public Layer
    {
        public:
            ImGuiLayer(Application* app);
            ~ImGuiLayer();

            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnEvent(Event& e);
        
        private:
            bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
            bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
            bool OnMouseMovedEvent(MouseMovedEvent& e);
            bool OnMouseScrolledEvent(MouseScrolledEvent& e);
            bool OnKeyPressedEvent(KeyPressedEvent& e);
            bool OnKeyReleasedEvent(KeyReleasedEvent& e);
            bool OnKeyTypedEvent(KeyTypedEvent& e);
            bool OnWindowResizeEvent(WindowResizeEvent& e);
            
        private:
            float m_Time;
            Application* m_Application;
    };
}