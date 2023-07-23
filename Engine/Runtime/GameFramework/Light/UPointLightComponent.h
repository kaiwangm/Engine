#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Renderer/Renderer.h>

namespace Engine
{
    class UPointLightComponent final : public UComponent
    {
    private:
        glm::vec3 m_Color;
        float     m_Intensity;

    public:
        Ref<ShadowCubeMapBuffer> m_ShadowCubeMapBuffer;

    public:
        UPointLightComponent() : m_Color(1.0f), m_Intensity(1.0f)
        {
            m_ShadowCubeMapBuffer = ShadowCubeMapBuffer::Create();
        }
        void SetColor(const glm::vec3& color) { m_Color = color; }
        void SetIntensity(const float intensity) { m_Intensity = intensity; }

        glm::vec3& GetColorRef() { return m_Color; }
        float&     GetIntensityRef() { return m_Intensity; }

        ShadowCubeMapBuffer& GetShadowCubeMapBufferRef() { return *m_ShadowCubeMapBuffer; }
        void SetShadowCubeMapViewPort(const uint32_t width, const uint32_t height) { m_ShadowCubeMapBuffer->SetViewPort(width, height); }
    };
} // namespace Engine