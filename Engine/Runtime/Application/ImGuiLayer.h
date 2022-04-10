#pragma once

#include "Core.h"
#include "Layer/Layer.h"

namespace Engine {
class ImGuiLayer : public Layer {
   public:
    virtual ~ImGuiLayer() = default;

    virtual void Begin() = 0;
    virtual void End() = 0;

    static Ref<ImGuiLayer> Create(const std::string& name);
};
}  // namespace Engine