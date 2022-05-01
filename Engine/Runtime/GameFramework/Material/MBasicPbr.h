#pragma once
#include "MMaterial.h"

namespace Engine
{
    class MBasicPbr final : public MMaterial
    {
    private:
        glm::vec3 m_Albedo;
        float     m_Metallic;
        float     m_Roughness;
        float     m_AO;

    public:
        MBasicPbr(const std::string& name) :
            MMaterial(name), m_Albedo(1.0f), m_Metallic(0.0f), m_Roughness(0.0f), m_AO(1.0f)
        {}
        ~MBasicPbr() = default;

        void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
        void SetMetallic(const float metallic) { m_Metallic = metallic; }
        void SetRoughness(const float roughness) { m_Roughness = roughness; }
        void SetAO(const float ao) { m_AO = ao; }

        glm::vec3& GetAlbedoRef() { return m_Albedo; }
        float&     GetMetallicRef() { return m_Metallic; }
        float&     GetRoughnessRef() { return m_Roughness; }
        float&     GetAORef() { return m_AO; }
    };
} // namespace Engine