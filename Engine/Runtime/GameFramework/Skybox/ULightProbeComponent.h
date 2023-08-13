#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Renderer/Texture.h>

namespace Engine
{
    class ULightProbeComponent final : public UComponent
    {
    public:
        glm::vec3                m_Position;
        Ref<CubeMap>             m_Probe_CubeMap;
        std::array<glm::vec3, 9> m_sphereHarmonicsParameters;

    public:
        ULightProbeComponent() { m_Probe_CubeMap = CubeMap::Create(); }

        void       SetProbePosition(const glm::vec3& position) { m_Position = position; }
        glm::vec3& GetProbePositionRef() { return m_Position; }
        glm::vec3  GetProbePosition() const { return m_Position; }

        Ref<CubeMap>& GetCubeMapPtr() { return m_Probe_CubeMap; }

        void SetSphereHarmonicsParameters(const std::array<glm::vec3, 9>& sphereHarmonicsParameters)
        {
            m_sphereHarmonicsParameters = sphereHarmonicsParameters;
        }
        std::array<glm::vec3, 9>& GetSphereHarmonicsParametersRef() { return m_sphereHarmonicsParameters; }
        std::array<glm::vec3, 9>  GetSphereHarmonicsParameters() const { return m_sphereHarmonicsParameters; }
    };
}; // namespace Engine