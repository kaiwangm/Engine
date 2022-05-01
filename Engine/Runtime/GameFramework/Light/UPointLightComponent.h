#pragma once
#include "../Common/UComponent.h"

namespace Engine
{
    class UPointLightComponent final : public UComponent
    {
    private:
        glm::vec3 m_Color;
        float     m_Intensity;

    public:
        UPointLightComponent() : m_Color(1.0f), m_Intensity(1.0f) {}
        void SetColor(const glm::vec3& color) { m_Color = color; }
        void SetIntensity(const float intensity) { m_Intensity = intensity; }

        glm::vec3& GetColorRef() { return m_Color; }
        float&     GetIntensityRef() { return m_Intensity; }
    };
} // namespace Engine