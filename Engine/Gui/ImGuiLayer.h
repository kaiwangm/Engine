#pragma once

#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "Layer.h"
#include "MouseEvent.h"

namespace Engine {
class ImGuiLayer : public Layer {
   public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& e) override;

    void Begin();
    void End();

   private:
        void SetDarkThemeColors();

   private:
    float m_Time;
};
}  // namespace Engine