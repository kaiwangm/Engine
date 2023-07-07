#pragma once

#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Core/Events/ApplicationEvent.h>
#include <Engine/Runtime/Core/Log/Log.h>

namespace Engine
{
    struct LayerUpdateMeta
    {
        float m_timeStep;
        float m_nowTime;
        LayerUpdateMeta() : m_timeStep(0.0f), m_nowTime(0.0f) {}
        LayerUpdateMeta(const float& timeStep, const float& nowTime) : m_timeStep(timeStep), m_nowTime(nowTime) {}
    };

    class Layer
    {
    public:
        Layer(const std::string& name = "Layer") { m_LayerName = name; };
        virtual ~Layer() {};

        virtual void OnAttach()            = 0;
        virtual void OnDetach()            = 0;
        virtual void OnEvent(Event& event) = 0;

        virtual void TickLogic() {};
        virtual void TickRender() {};
        virtual void TickGui() {};

        inline const std::string& GetName() const { return m_LayerName; }

        void SetLayerUpdateMeta(const LayerUpdateMeta& meta) { m_LayerUpdateMeta = meta; }

    protected:
        std::string     m_LayerName;
        LayerUpdateMeta m_LayerUpdateMeta;
        std::unordered_map<std::string, std::function<void()>> m_Callbacks;
    };
} // namespace Engine