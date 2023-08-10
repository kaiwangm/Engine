#pragma once
#include <Engine/Runtime/GameFramework/Material/MMaterial.h>
#include <Engine/Runtime/Renderer/Texture.h>

namespace Engine
{
    class MBasicPbr final : public MMaterial
    {
    private:
        glm::vec3 m_Albedo    = glm::vec3(0.0f);
        float     m_Metallic  = 0.0f;
        float     m_Roughness = 0.0f;
        float     m_AO        = 0.0f;
        glm::vec3 m_Diffuse   = glm::vec3(0.0f);
        glm::vec3 m_Specular  = glm::vec3(0.0f);
        glm::vec3 m_Opacity   = glm::vec3(1.0f);

        Ref<Texture2D> m_AlbedoMap;
        Ref<Texture2D> m_NormalMap;
        Ref<Texture2D> m_MetallicMap;
        Ref<Texture2D> m_RoughnessMap;
        Ref<Texture2D> m_AOMap;
        Ref<Texture2D> m_DiffuseMap;
        Ref<Texture2D> m_SpecularMap;
        Ref<Texture2D> m_OpacityMap;

        int m_Workflow = 0;

        bool m_UseAlbedoMap    = false;
        bool m_UseNormalMap    = false;
        bool m_UseMetallicMap  = false;
        bool m_UseRoughnessMap = false;
        bool m_UseAOMap        = false;
        bool m_UseDiffuseMap   = false;
        bool m_UseSpecularMap  = false;
        bool m_UseOpacityMap   = false;

        bool      m_EnableEmissive    = false;
        float     m_EmissiveIntensity = 0.0f;
        glm::vec3 m_Emissive          = glm::vec3(0.0f);

        int m_DiffuseMapChannels  = 0;
        int m_SpecularMapChannels = 0;

        bool isBuffered = false;

    public:
        MBasicPbr(const std::string& name) : MMaterial(name, "BasicPbr") {}

        MBasicPbr(const std::string& name, const std::string& folderName) : MMaterial(name, "BasicPbr")
        {
            m_Workflow = 0;

            LoadAlbedoMap("Assets/Editor/Material/" + folderName + "/" + folderName + "_albedo.png");
            LoadNormalMap("Assets/Editor/Material/" + folderName + "/" + folderName + "_normal.png");
            LoadMetallicMap("Assets/Editor/Material/" + folderName + "/" + folderName + "_metallic.png");
            LoadRoughnessMap("Assets/Editor/Material/" + folderName + "/" + folderName + "_roughness.png");
            LoadAOMap("Assets/Editor/Material/" + folderName + "/" + folderName + "_ao.png");
        }

        virtual ~MBasicPbr() override {};

        void SetWorkflow(const int workflow) { m_Workflow = workflow; }
        int& GetWorkflowRef() { return m_Workflow; }

        void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
        void SetMetallic(const float metallic) { m_Metallic = metallic; }
        void SetRoughness(const float roughness) { m_Roughness = roughness; }
        void SetAO(const float ao) { m_AO = ao; }
        void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
        void SetSpecular(const glm::vec3& specular) { m_Specular = specular; }
        void SetDiffuseMapChannels(const int channels) { m_DiffuseMapChannels = channels; }
        void SetSpecularMapChannels(const int channels) { m_SpecularMapChannels = channels; }
        void SetEnableEmissive(const bool enable) { m_EnableEmissive = enable; }
        void SetEmissive(const glm::vec3& emissive) { m_Emissive = emissive; }
        void SetEmissiveIntensity(const float power) { m_EmissiveIntensity = power; }
        void SetOpacity(const glm::vec3& opacity) { m_Opacity = opacity; }

        glm::vec3& GetAlbedoRef() { return m_Albedo; }
        float&     GetMetallicRef() { return m_Metallic; }
        float&     GetRoughnessRef() { return m_Roughness; }
        float&     GetAORef() { return m_AO; }
        glm::vec3& GetDiffuseRef() { return m_Diffuse; }
        glm::vec3& GetSpecularRef() { return m_Specular; }
        int&       GetDiffuseMapChannelsRef() { return m_DiffuseMapChannels; }
        int&       GetSpecularMapChannelsRef() { return m_SpecularMapChannels; }
        void       SetEnableEmissiveRef(const bool enable) { m_EnableEmissive = enable; }
        glm::vec3& GetEmissiveRef() { return m_Emissive; }
        float&     GetEmissiveIntensityRef() { return m_EmissiveIntensity; }
        glm::vec3& GetOpacityRef() { return m_Opacity; }

        void LoadAlbedoMap(const std::string& path)
        {
            m_AlbedoMap    = Texture2D::Create(path);
            m_UseAlbedoMap = true;
        }
        void LoadNormalMap(const std::string& path)
        {
            m_NormalMap    = Texture2D::Create(path);
            m_UseNormalMap = true;
        }
        void LoadMetallicMap(const std::string& path)
        {
            m_MetallicMap    = Texture2D::Create(path);
            m_UseMetallicMap = true;
        }
        void LoadRoughnessMap(const std::string& path)
        {
            m_RoughnessMap    = Texture2D::Create(path);
            m_UseRoughnessMap = true;
        }
        void LoadAOMap(const std::string& path)
        {
            m_AOMap    = Texture2D::Create(path);
            m_UseAOMap = true;
        }
        void LoadDiffuseMap(const std::string& path)
        {
            m_DiffuseMap         = Texture2D::Create(path);
            m_UseDiffuseMap      = true;
            m_DiffuseMapChannels = m_DiffuseMap->GetChannels();
        }
        void LoadSpecularMap(const std::string& path)
        {
            m_SpecularMap         = Texture2D::Create(path);
            m_UseSpecularMap      = true;
            m_SpecularMapChannels = m_SpecularMap->GetChannels();
        }
        void LoadOpacityMap(const std::string& path)
        {
            m_OpacityMap    = Texture2D::Create(path);
            m_UseOpacityMap = true;
        }

        virtual void BufferTextures() override
        {
            if (isBuffered == true)
            {
                return;
            }

            if (m_UseAlbedoMap == true)
            {
                m_AlbedoMap->Buffer();
            }
            if (m_UseNormalMap == true)
            {
                m_NormalMap->Buffer();
            }
            if (m_UseMetallicMap == true)
            {
                m_MetallicMap->Buffer();
            }
            if (m_UseRoughnessMap == true)
            {
                m_RoughnessMap->Buffer();
            }
            if (m_UseAOMap == true)
            {
                m_AOMap->Buffer();
            }
            if (m_UseDiffuseMap == true)
            {
                m_DiffuseMap->Buffer();
            }
            if (m_UseSpecularMap == true)
            {
                m_SpecularMap->Buffer();
            }
            if (m_UseOpacityMap == true)
            {
                m_OpacityMap->Buffer();
            }

            isBuffered = true;
        }

        virtual void ClearTextures() override
        {
            if (isBuffered == false)
            {
                return;
            }

            if (m_UseAlbedoMap == true)
            {
                m_AlbedoMap->Clear();
            }
            if (m_UseNormalMap == true)
            {
                m_NormalMap->Clear();
            }
            if (m_UseMetallicMap == true)
            {
                m_MetallicMap->Clear();
            }
            if (m_UseRoughnessMap == true)
            {
                m_RoughnessMap->Clear();
            }
            if (m_UseAOMap == true)
            {
                m_AOMap->Clear();
            }
            if (m_UseDiffuseMap == true)
            {
                m_DiffuseMap->Clear();
            }
            if (m_UseSpecularMap == true)
            {
                m_SpecularMap->Clear();
            }
            if (m_UseOpacityMap == true)
            {
                m_OpacityMap->Clear();
            }

            isBuffered = false;
        }

        void BindAllMap(const Ref<Shader> shader)
        {
            shader->SetInt("workflow", m_Workflow);

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

            shader->SetFloat3("in_diffuse", m_Diffuse);
            shader->SetBool("useDiffuseMap", m_UseDiffuseMap);
            shader->SetInt("diffuseMap", 5);

            shader->SetFloat3("in_specular", m_Specular);
            shader->SetBool("useSpecularMap", m_UseSpecularMap);
            shader->SetInt("specularMap", 6);

            shader->SetInt("diffuseMapChannels", m_DiffuseMapChannels);
            shader->SetInt("specularMapChannels", m_SpecularMapChannels);

            shader->SetFloat3("in_opacity", m_Opacity);
            shader->SetBool("useOpacityMap", m_UseOpacityMap);
            shader->SetInt("opacityMap", 7);

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
            if (m_UseDiffuseMap == true)
            {
                m_DiffuseMap->Bind(5);
            }
            if (m_UseSpecularMap == true)
            {
                m_SpecularMap->Bind(6);
            }
            if (m_UseOpacityMap == true)
            {
                m_OpacityMap->Bind(7);
            }

            shader->SetBool("enableEmissive", m_EnableEmissive);
            shader->SetFloat3("in_emissive", m_Emissive);
            shader->SetFloat("in_emissiveIntensity", m_EmissiveIntensity);
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
            if (m_UseDiffuseMap == true)
            {
                m_DiffuseMap->UnBind(5);
            }
            if (m_UseSpecularMap == true)
            {
                m_SpecularMap->UnBind(6);
            }
            if (m_UseOpacityMap == true)
            {
                m_OpacityMap->UnBind(7);
            }
        }
    };
} // namespace Engine