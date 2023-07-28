#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Renderer/Renderer.h>

namespace Engine
{
    class UDirectionalLightComponent final : public UComponent
    {
    private:
        glm::vec3 m_Color;
        glm::vec3 m_Direction;
        float     m_Intensity;

    public:
        Ref<ShadowMapBuffer> m_ShadowMapBuffer;

    public:
        UDirectionalLightComponent() : m_Color(1.0f), m_Intensity(1.0f)
        {
            m_ShadowMapBuffer = ShadowMapBuffer::Create();
        }
        void SetColor(const glm::vec3& color) { m_Color = color; }
        void SetDirection(const glm::vec3& direction) { m_Direction = glm::normalize(direction); }
        void SetIntensity(const float intensity) { m_Intensity = intensity; }

        glm::vec3& GetColorRef() { return m_Color; }
        glm::vec3& GetDirectionRef() { return m_Direction; }
        float&     GetIntensityRef() { return m_Intensity; }

        ShadowMapBuffer& GetShadowMapBufferRef() { return *m_ShadowMapBuffer; }
        void             SetShadowMapViewPort(const uint32_t width, const uint32_t height)
        {
            m_ShadowMapBuffer->SetViewPort(width, height);
        }
    };
} // namespace Engine