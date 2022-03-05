#pragma once
#include "Core.h"
#include "Layer.h"

namespace Engine {
class DockSpaceLayer : public Layer {
   public:
    DockSpaceLayer(bool& app_open_ref)
        : Layer("DockSpaceLayer"), m_app_open(app_open_ref) {}
    virtual ~DockSpaceLayer() = default;

    virtual void OnAttach() override {}
    virtual void OnDetach() override {}
    virtual void OnUpdate() override {}
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& event) override {}

   private:
    bool& m_app_open;
};
}  // namespace Engine