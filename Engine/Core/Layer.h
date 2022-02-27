#pragma once

#include "ApplicationEvent.h"
#include "Core.h"
#include "Log.h"

namespace Engine {
class Layer {
   public:
    Layer(const std::string& name = "Layer") { m_LayerName = name; };
    virtual ~Layer() {}

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}

    virtual void OnEvent(Event& event) {}

    inline const std::string& GetName() const { return m_LayerName; }

   protected:
    std::string m_LayerName;
};
}  // namespace Engine