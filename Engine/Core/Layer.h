#pragma once

#include "ApplicationEvent.h"
#include "Core.h"
#include "Log.h"

namespace Engine {
struct LayerUpdateMeta {
    float m_timeStep;
    float m_nowTime;
    LayerUpdateMeta() : m_timeStep(0.0f), m_nowTime(0.0f) {}
    LayerUpdateMeta(const float& timeStep, const float& nowTime)
        : m_timeStep(timeStep), m_nowTime(nowTime) {}
};

class Layer {
   public:
    Layer(const std::string& name = "Layer") { m_LayerName = name; };
    virtual ~Layer() {};

    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnImGuiRender() = 0;
    virtual void OnEvent(Event& event) = 0;

    inline const std::string& GetName() const { return m_LayerName; }

    void SetLayerUpdateMeta(const LayerUpdateMeta& meta)
    {
        m_LayerUpdateMeta = meta;
    }

   protected:
    std::string m_LayerName;
    LayerUpdateMeta m_LayerUpdateMeta;
};
}  // namespace Engine