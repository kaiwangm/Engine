#pragma once
#include <Engine/Runtime/GameFramework/Material/MMaterial.h>
#include <Engine/Runtime/Renderer/Texture.h>

namespace Engine
{
    class MBasicPbr final : public MMaterial
    {
    private:
        glm::vec3 m_Albedo;
        float     m_Metallic;
        float     m_Roughness;
        float     m_AO;

        Ref<Texture2D> m_AlbedoMap;
        Ref<Texture2D> m_NormalMap;
        Ref<Texture2D> m_MetallicMap;
        Ref<Texture2D> m_RoughnessMap;
        Ref<Texture2D> m_AOMap;

        bool m_UseAlbedoMap;
        bool m_UseNormalMap;
        bool m_UseMetallicMap;
        bool m_UseRoughnessMap;
        bool m_UseAOMap;

    public:
        MBasicPbr(const std::string& name) :
            MMaterial(name, "BasicPbr"), m_Albedo(1.0f), m_Metallic(0.0f), m_Roughness(1.0f), m_AO(1.0f)
        {
            m_UseAlbedoMap    = false;
            m_UseNormalMap    = false;
            m_UseMetallicMap  = false;
            m_UseRoughnessMap = false;
            m_UseAOMap        = false;
        }

        MBasicPbr(const std::string& name, const std::string& folderName) :
            MMaterial(name, "BasicPbr"), m_Albedo(1.0f), m_Metallic(0.0f), m_Roughness(0.0f), m_AO(1.0f)
        {
            m_AlbedoMap    = Texture2D::Create("Assert/Material/" + folderName + "/" + folderName + "_albedo.png");
            m_NormalMap    = Texture2D::Create("Assert/Material/" + folderName + "/" + folderName + "_normal.png");
            m_MetallicMap  = Texture2D::Create("Assert/Material/" + folderName + "/" + folderName + "_metallic.png");
            m_RoughnessMap = Texture2D::Create("Assert/Material/" + folderName + "/" + folderName + "_roughness.png");
            m_AOMap        = Texture2D::Create("Assert/Material/" + folderName + "/" + folderName + "_ao.png");

            m_UseAlbedoMap    = true;
            m_UseNormalMap    = true;
            m_UseMetallicMap  = true;
            m_UseRoughnessMap = true;
            m_UseAOMap        = true;
        }

        ~MBasicPbr() = default;

        void Bind(const Ref<Shader> shader)
        {
            Renderer::SetShaderUniform(shader, "in_albedo", m_Albedo);
            Renderer::SetShaderUniform(shader, "in_roughness", m_Roughness);
            Renderer::SetShaderUniform(shader, "in_metallic", m_Metallic);
            Renderer::SetShaderUniform(shader, "in_ao", m_AO);

            Renderer::SetShaderUniform(shader, "albedoMap", 0);
            Renderer::SetShaderUniform(shader, "normalMap", 1);
            Renderer::SetShaderUniform(shader, "metallicMap", 2);
            Renderer::SetShaderUniform(shader, "roughnessMap", 3);
            Renderer::SetShaderUniform(shader, "aoMap", 4);

            Renderer::SetShaderUniform(shader, "irradianceMap", 5);
            Renderer::SetShaderUniform(shader, "prefilterMap", 6);
            Renderer::SetShaderUniform(shader, "brdfLUT", 7);

            Renderer::SetShaderUniform(shader, "useAlbedoMap", m_UseAlbedoMap);
            Renderer::SetShaderUniform(shader, "useNormalMap", m_UseNormalMap);
            Renderer::SetShaderUniform(shader, "useMetallicMap", m_UseMetallicMap);
            Renderer::SetShaderUniform(shader, "useRoughnessMap", m_UseRoughnessMap);
            Renderer::SetShaderUniform(shader, "useAOMap", m_UseAOMap);

            if (m_UseAlbedoMap == true)
            {
                m_AlbedoMap->Bind(0);
            }
            if (m_UseNormalMap == true)
            {
                m_NormalMap->Bind(1);
            }
            if (m_UseMetallicMap == true)
            {
                m_MetallicMap->Bind(2);
            }
            if (m_UseRoughnessMap == true)
            {
                m_RoughnessMap->Bind(3);
            }
            if (m_UseAOMap == true)
            {
                m_AOMap->Bind(4);
            }
        }

        void UnBind(const Ref<Shader> shader)
        {
            if (m_UseAOMap == true)
            {
                m_AOMap->UnBind(4);
            }
            if (m_UseRoughnessMap == true)
            {
                m_RoughnessMap->UnBind(3);
            }
            if (m_UseMetallicMap == true)
            {
                m_MetallicMap->UnBind(2);
            }
            if (m_UseNormalMap == true)
            {
                m_NormalMap->UnBind(1);
            }
            if (m_UseAlbedoMap == true)
            {
                m_AlbedoMap->UnBind(0);
            }
        }

        void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
        void SetMetallic(const float metallic) { m_Metallic = metallic; }
        void SetRoughness(const float roughness) { m_Roughness = roughness; }
        void SetAO(const float ao) { m_AO = ao; }

        glm::vec3& GetAlbedoRef() { return m_Albedo; }
        float&     GetMetallicRef() { return m_Metallic; }
        float&     GetRoughnessRef() { return m_Roughness; }
        float&     GetAORef() { return m_AO; }

        void BindAllMap(const Ref<Shader> shader)
        {
            shader->SetFloat3("in_albedo", m_Albedo);
            shader->SetBool("useAlbedoMap", m_UseAlbedoMap);
            shader->SetInt("albedoMap", 0);

            shader->SetBool("useNormalMap", m_UseNormalMap);
            shader->SetInt("normalMap", 1);

            shader->SetFloat("in_roughness", m_Roughness);
            shader->SetBool("useRoughnessMap", m_UseRoughnessMap);
            shader->SetInt("roughnessMap", 2);

            shader->SetFloat("in_metallic", m_Metallic);
            shader->SetBool("useMetallicMap", m_UseMetallicMap);
            shader->SetInt("metallicMap", 3);

            shader->SetFloat("in_ao", m_AO);
            shader->SetBool("useAOMap", m_UseAOMap);
            shader->SetInt("aoMap", 4);

            if (m_UseAlbedoMap == true)
            {
                m_AlbedoMap->Bind(0);
            }
            if (m_UseNormalMap == true)
            {
                m_NormalMap->Bind(1);
            }
            if (m_UseRoughnessMap == true)
            {
                m_RoughnessMap->Bind(2);
            }
            if (m_UseMetallicMap == true)
            {
                m_MetallicMap->Bind(3);
            }
            if (m_UseAOMap == true)
            {
                m_AOMap->Bind(4);
            }
        }

        void UnBindAllMap(const Ref<Shader> shader)
        {
            if (m_UseAlbedoMap == true)
            {
                m_AlbedoMap->UnBind(0);
            }
            if (m_UseNormalMap == true)
            {
                m_NormalMap->UnBind(1);
            }
            if (m_UseMetallicMap == true)
            {
                m_MetallicMap->UnBind(2);
            }
            if (m_UseRoughnessMap == true)
            {
                m_RoughnessMap->UnBind(3);
            }
            if (m_UseAOMap == true)
            {
                m_AOMap->UnBind(4);
            }
        }
    };
} // namespace Engine