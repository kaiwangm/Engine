#pragma once

#include "Layer.h"

#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Engine 
{
    class ImGuiLayer : public Layer
    {
        public:
            ImGuiLayer();
            ~ImGuiLayer();

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate() override;
            
            virtual void OnEvent(Event& e) override;
        
        private:
            void OnImGuiRender();

            void Begin();
            void End();
            void SetDarkThemeColors();
            
        private:
            float m_Time;
    };
}