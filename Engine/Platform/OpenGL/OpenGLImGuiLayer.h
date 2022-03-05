#pragma once
#include "Core.h"
#include "ImGuiLayer.h"

namespace Engine {
class OpenGLImGuiLayer : public ImGuiLayer {
   public:
    OpenGLImGuiLayer(const std::string& name);
    virtual ~OpenGLImGuiLayer();
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& event) override;

    virtual void Begin() override;
    virtual void End() override;

   private:
    void SetDarkThemeColors();
};
}  // namespace Engine