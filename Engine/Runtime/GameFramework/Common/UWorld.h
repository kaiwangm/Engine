#pragma once
#include <Engine/Runtime/Core/Core.h>

#include <entt/entt.hpp>
#include <vector>

#include <Engine/Runtime/GameFramework/Common/UObject.h>
#include <Engine/Runtime/GameFramework/Camera/UCameraComponent.h>
#include <Engine/Runtime/Renderer/Renderer.h>

#include <Engine/Runtime/GameFramework/Skybox/ULightProbeComponent.h>
#include <Engine/Runtime/GameFramework/Skybox/USkyboxComponent.h>

namespace Engine
{
    class AActor;
    class ACamera;
    class ASkybox;

    class UWorld : public UObject
    {
    private:
        std::vector<Ref<AActor>> m_Actors;
        entt::registry           m_Registry;
        ShaderLibrary            m_ShaderLibrary;

    public:
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_skybox;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_DirectLighting_diffuse;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_DirectLighting_specular;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_EnvironmentLighting_diffuse;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_EnvironmentLighting_specular;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_VoxelGI;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_playground;

        Ref<FrameRenderBuffer>                m_FrameRenderBuffer_ssr;
        Ref<FrameRenderBuffer>                m_FrameRenderBuffer_ssr_blur;
        Ref<FrameRenderBuffer>                m_FrameRenderBuffer_exposure;
        Ref<FrameRenderBuffer>                m_FrameRenderBuffer;
        Ref<FrameRenderBuffer>                m_FrameRenderBuffer_highLight;
        std::array<Ref<FrameRenderBuffer>, 7> m_FrameRenderBuffer_highLight_downSampled;
        std::array<Ref<FrameRenderBuffer>, 6> m_FrameRenderBuffer_highLight_blur;
        std::array<Ref<FrameRenderBuffer>, 6> m_FrameRenderBuffer_highLight_upSampled;
        Ref<FrameRenderBuffer>                m_FrameRenderBuffer_bloom;
        Ref<FrameRenderBuffer>                m_FrameRenderBuffer_fxaa;

        Ref<FrameRenderBuffer> m_FrameRenderBuffer_gui;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_bufferViewport;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_shadowMapViewport;

        Ref<FrameRenderBuffer> m_FrameRenderBuffer_VoxelGIViewport;

        Ref<ULightProbeComponent> m_LightProbeComponent;

        const char* viewport_items[23] = {
            "ViewPosition",
            "ViewNormal",
            "Diffuse",
            "Depth",
            "Ambient Occlusion",
            "Roughness",
            "Specular",
            "WorldPosition",
            "WorldNormal",
            "Emissive",
            "Skybox",
            "DirectLighting_Diffuse",
            "DirectLighting_Specular",
            "EnvironmentLighting_Diffuse",
            "EnvironmentLighting_Specular",
            "ScreenSpaceReflection",
            "ScreenSpaceReflection_Blur",
            "Bloom",
            "Exposure",
            "VoxelGI_VoxelTexture",
            "VoxelGI",
            "ForwardUI",
            "Viewport",
        };

        Ref<GeometryBuffer> m_GeometryBuffer;
        Ref<SSAOBuffer>     m_SSAOBuffer;
        Ref<Texture3D>      m_VoxelGI_VoxelTexture;

        glm::vec4 m_BackGroundColor {0.0f, 0.0f, 0.0f, 1.0f};

        UCameraComponent* m_MainCamera;
        AActor*           m_ControlledActor;
        ASkybox*          m_MainSkybox;

        glm::mat4 m_VMatrix;
        glm::mat4 m_PMatrix;
        glm::mat4 m_VPMatrix;

        float m_Exposure           = 1.5f;
        int   m_ViewportGBufferMap = 0;

        float m_Dir_Diffuse  = 0.20f;
        float m_Dir_Specular = 0.20f;
        float m_Env_Diffuse  = 0.20f;
        float m_Env_Specular = 0.20f;
        float m_VoxelGI      = 0.20f;
        float m_SSR_Factor   = 0.20f;

        struct ScreenSpaceReflection
        {
            bool  debug                             = false;
            float rayStep                           = 0.03f;
            float minRayStep                        = 0.1f;
            float maxSteps                          = 300.0f;
            int   numBinarySearchSteps              = 5;
            float reflectionSpecularFalloffExponent = 0.3f;
            float refBias                           = 0.001f;
        } m_SSR_settings;

        float m_Bloom_Intensity = 0.18f;

        struct Shadow
        {
            bool  rebuild_shadow_map = true;
            float PCSS_FilterRadius  = 9.0f;
        } m_Shadow_settings;

        struct VoxelGI
        {
            struct ConeTracingSettings
            {
                float aperture;
                float sampling_factor;
                float distance_offset;
                float max_distance;
                float result_intensity;
            };

            bool rebuild_voxel_grid    = true;
            int  voxel_grid_resolution = 300;
            int  max_mipmap_level      = 8;
            // glm::vec3           scene_voxel_scale     = glm::vec3(1.0f) / 75.0f;
            glm::vec3           scene_voxel_scale = glm::vec3(1.0f) / 75.0f;
            ConeTracingSettings diffuse           = {0.300f, 0.300f, 0.050f, 2.0f, 1.0f};
            ConeTracingSettings specular          = {0.300f, 0.300f, 0.050f, 2.0f, 1.0f};
        } m_VoxelGI_settings;

        struct LightingProbe
        {
            bool rebuild_lighting_probe = true;
        } m_LightingProbe_settings;

        entt::entity entity_selected = entt::null;

    public:
        bool m_RenderSkybox = true;
        bool m_RenderGrid   = false;
        bool m_RenderGizmo  = true;

    public:
        UWorld();
        void BuildLightingProbe();

        template<class T, class... Args>
        T& AddActor(const std::string& name, Args... args)
        {
            Ref<T> actor = std::make_shared<T>(this, m_Registry.create(), name, args...);
            m_Actors.push_back(actor);

            return *actor;
        }

        template<class T, class... Args>
        T* RegisterComponents(entt::entity handle, Args... args)
        {
            T& prt = m_Registry.emplace<T>(handle, args...);

            return &prt;
        }

        void LoadShader(const std::string& name,
                        const std::string& vertexSrc,
                        const std::string& fragmentSrc,
                        const std::string& mode)
        {
            m_ShaderLibrary.Load(name, vertexSrc, fragmentSrc, mode);
        }

        void LoadShader(const std::string& name,
                        const std::string& vertexSrc,
                        const std::string& geometrySrc,
                        const std::string& fragmentSrc,
                        const std::string& mode)
        {
            m_ShaderLibrary.Load(name, vertexSrc, geometrySrc, fragmentSrc, mode);
        }

        void TickLogic(float timeStep, float nowTime, bool isWindowFocused);
        void TickRender(float timeStep);
        void TickGui(float timeStep);

        entt::registry& GetRegistry() { return m_Registry; }
    };
}; // namespace Engine