#include <Engine/Runtime/GameFramework/Common/UWorld.h>

#include <memory>

#include <Engine/Runtime/Renderer/Shader.h>
#include <Engine/Runtime/Renderer/Buffer.h>
#include <Engine/Runtime/Gui/GuiCommand.h>
#include <Engine/Runtime/Core/Input/Input.h>
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/GameFramework/Camera/UCameraComponent.h>
#include <Engine/Runtime/GameFramework/StaticMesh/AStaticMesh.h>
#include <Engine/Runtime/GameFramework/StaticMesh/UChunkArrayWorldComponent.h>
#include <Engine/Runtime/GameFramework/StaticMesh/APointCloud.h>
#include <Engine/Runtime/GameFramework/Animation/ASkeleton.h>
#include <Engine/Runtime/GameFramework/Animation/ASkinnedMesh.h>
#include <Engine/Runtime/GameFramework/Animation/UAnimatedMeshComponent.h>
#include <Engine/Runtime/GameFramework/Animation/USkeletonComponent.h>
#include <Engine/Runtime/GameFramework/Animation/USkinnedMeshComponent.h>
#include <Engine/Runtime/GameFramework/Animation/UMotionMatchingComponent.h>
#include <Engine/Runtime/GameFramework/Skybox/ASkybox.h>
#include <Engine/Runtime/GameFramework/Camera/ACamera.h>
#include <Engine/Runtime/GameFramework/Pawn/APawn.h>
#include <Engine/Runtime/GameFramework/Light/UPointLightComponent.h>
#include <Engine/Runtime/GameFramework/Light/UDirectionalLightComponent.h>

namespace Engine
{
    UWorld::UWorld()
    {
        // Create Buffer
        m_FrameRenderBuffer_skybox                       = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_DirectLighting_diffuse       = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_DirectLighting_specular      = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_EnvironmentLighting_diffuse  = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_EnvironmentLighting_specular = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_VoxelGI                      = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_playground                   = FrameRenderBuffer::Create();

        m_FrameRenderBuffer_ssr       = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_ssr_blur  = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_exposure  = FrameRenderBuffer::Create();
        m_FrameRenderBuffer           = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_highLight = FrameRenderBuffer::Create();
        for (int i = 0; i < 7; ++i)
        {
            m_FrameRenderBuffer_highLight_downSampled[i] = FrameRenderBuffer::Create();
        }

        for (int i = 0; i < 6; ++i)
        {
            m_FrameRenderBuffer_highLight_blur[i]      = FrameRenderBuffer::Create();
            m_FrameRenderBuffer_highLight_upSampled[i] = FrameRenderBuffer::Create();
        }
        m_FrameRenderBuffer_bloom = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_fxaa  = FrameRenderBuffer::Create();

        m_FrameRenderBuffer_bufferViewport        = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_shadowMapViewport     = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_shadowCubeMapViewport = FrameRenderBuffer::Create();

        m_FrameRenderBuffer_VoxelGIViewport = FrameRenderBuffer::Create();

        // GeometryBuffer
        m_GeometryBuffer       = GeometryBuffer::Create();
        m_SSAOBuffer           = SSAOBuffer::Create();
        m_VoxelGI_VoxelTexture = Texture3D::Create(m_VoxelGI_settings.voxel_grid_resolution,
                                                   m_VoxelGI_settings.voxel_grid_resolution,
                                                   m_VoxelGI_settings.voxel_grid_resolution);

        LoadShader("TextureShader", "Assets/Editor/Shader/vertex.glsl", "Assets/Editor/Shader/fragment.glsl", "Path");
        LoadShader("Animated",
                   "Assets/Editor/Shader/vertex_animated.glsl",
                   "Assets/Editor/Shader/fragment_animated.glsl",
                   "Path");
        LoadShader(
            "Skybox", "Assets/Editor/Shader/vertex_skybox.glsl", "Assets/Editor/Shader/fragment_skybox.glsl", "Path");
        LoadShader("BasicPbr",
                   "Assets/Editor/Shader/vertex_basicpbr.glsl",
                   "Assets/Editor/Shader/fragment_basicpbr.glsl",
                   "Path");
        LoadShader("OctreeShader",
                   "Assets/Editor/Shader/octree_vertex.glsl",
                   "Assets/Editor/Shader/octree_fragment.glsl",
                   "Path");
        LoadShader("TriangleShader",
                   "Assets/Editor/Shader/triangle_vertex.glsl",
                   "Assets/Editor/Shader/triangle_fragment.glsl",
                   "Path");
        LoadShader("Skeleton",
                   "Assets/Editor/Shader/skeleton_vertex.glsl",
                   "Assets/Editor/Shader/skeleton_fragment.glsl",
                   "Path");
        LoadShader(
            "GBuffer", "Assets/Editor/Shader/deffered/gbuffer.vs", "Assets/Editor/Shader/deffered/gbuffer.fs", "Path");

        LoadShader("VisPosition",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/position.glsl",
                   "Path");

        LoadShader("VisNormal",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/normal.glsl",
                   "Path");

        LoadShader("VisAlbedo",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/albedo.glsl",
                   "Path");

        LoadShader("VisOpacity",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/opacity.glsl",
                   "Path");

        LoadShader("VisDepth",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/depth.glsl",
                   "Path");

        LoadShader(
            "VisAO", "Assets/Editor/Shader/screen_quad_vertex.glsl", "Assets/Editor/Shader/viewport/ao.glsl", "Path");

        LoadShader("ComputeAO",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/compute_ssao_fragment.glsl",
                   "Path");

        LoadShader("VisRoughness",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/roughness.glsl",
                   "Path");

        LoadShader("VisMetallic",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/metallic.glsl",
                   "Path");

        LoadShader("VisEmissive",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/emissive.glsl",
                   "Path");

        LoadShader("Deferred",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/fragment.glsl",
                   "Path");

        LoadShader("Exposure",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/exposure.fs",
                   "Path");

        LoadShader("Pawn", "Assets/Editor/Shader/pawn_vertex.glsl", "Assets/Editor/Shader/pawn_fragment.glsl", "Path");

        LoadShader("Trajectory",
                   "Assets/Editor/Shader/trajectory_vertex.glsl",
                   "Assets/Editor/Shader/trajectory_fragment.glsl",
                   "Path");

        LoadShader("Skinned",
                   "Assets/Editor/Shader/skinned/vertex.glsl",
                   "Assets/Editor/Shader/gbuffer_fragment.glsl",
                   "Path");

        LoadShader("ScreenSpaceReflection",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/ssr.fs",
                   "Path");

        LoadShader("GaussianBlur",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/gaussian_blur.fs",
                   "Path");

        LoadShader("Screen",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/screen_quad_fragment.glsl",
                   "Path");

        LoadShader("DirectLighting_diffuse",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/directLighting_diffuse.fs",
                   "Path");

        LoadShader("DirectLighting_specular",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/directLighting_specular.fs",
                   "Path");

        LoadShader("EnvironmentLighting_diffuse",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/environmentLighting_diffuse.fs",
                   "Path");

        LoadShader("EnvironmentLighting_specular",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/environmentLighting_specular.fs",
                   "Path");

        LoadShader("Composite",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/composite.fs",
                   "Path");

        LoadShader("ShadowMap",
                   "Assets/Editor/Shader/shadowmap/vertex.glsl",
                   "Assets/Editor/Shader/shadowmap/fragment.glsl",
                   "Path");

        LoadShader("VisCubeDepth",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/viewport/depth_cubemap.glsl",
                   "Path");

        LoadShader("HighLight",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/highlight.fs",
                   "Path");

        LoadShader("Identity",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/identity.fs",
                   "Path");

        LoadShader("Addition",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/addition.fs",
                   "Path");

        LoadShader("ChunkGBuffer",
                   "Assets/Editor/Shader/chunk/gbuffer_vertex.vs",
                   "Assets/Editor/Shader/chunk/gbuffer_fragment.fs",
                   "Path");

        LoadShader("FXAA", "Assets/Editor/Shader/deffered/fxaa.vs", "Assets/Editor/Shader/deffered/fxaa.fs", "Path");

        LoadShader("VoxelGI_Voxelization",
                   "Assets/Editor/Shader/voxelgi/voxelization.vs",
                   "Assets/Editor/Shader/voxelgi/voxelization.gs",
                   "Assets/Editor/Shader/voxelgi/voxelization.fs",
                   "Path");

        LoadShader("VoxelGI_Visualization",
                   "Assets/Editor/Shader/voxelgi/visualization.vs",
                   "Assets/Editor/Shader/voxelgi/visualization.fs",
                   "Path");

        LoadShader("VoxelGI_Conetracing",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/voxelgi/conetracing.fs",
                   "Path");
    }

    void UWorld::Initialize()
    {
        auto actor_view           = m_Registry.view<UTagComponent, UTransformComponent>();
        auto camrea_view          = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto model_view           = m_Registry.view<UTagComponent, UTransformComponent, UStaticMeshComponent>();
        auto pointcloud_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointCloudComponent>();
        auto pointlight_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();
        auto dirctionallight_view = m_Registry.view<UTagComponent, UTransformComponent, UDirectionalLightComponent>();
        auto skybox_view          = m_Registry.view<UTagComponent, UTransformComponent, USkyboxComponent>();
        auto skeleton_view        = m_Registry.view<UTagComponent, UTransformComponent, USkeletonComponent>();
        auto pawn_view            = m_Registry.view<UTagComponent, UTransformComponent, UPawnComponent>();
        auto trajectory_view      = m_Registry.view<UTagComponent, UTransformComponent, UTrajectoryComponent>();
        auto skinnedmesh_view     = m_Registry.view<UTagComponent, UTransformComponent, USkinnedMeshComponent>();
        auto motionmatching_view  = m_Registry.view<UTagComponent, UTransformComponent, UMotionMatchingComponent>();

        Ref<GeometryBuffer>    m_GeometryBuffer_Baking                                 = GeometryBuffer::Create();
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_skybox_Baking                       = FrameRenderBuffer::Create();
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_DirectLighting_diffuse_Baking       = FrameRenderBuffer::Create();
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_DirectLighting_specular_Baking      = FrameRenderBuffer::Create();
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_EnvironmentLighting_diffuse_Baking  = FrameRenderBuffer::Create();
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_EnvironmentLighting_specular_Baking = FrameRenderBuffer::Create();
        Ref<SSAOBuffer>        m_SSAOBuffer_Baking                                     = SSAOBuffer::Create();
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_ssr_Baking                          = FrameRenderBuffer::Create();
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_ssr_blur_Baking                     = FrameRenderBuffer::Create();
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_Baking                              = FrameRenderBuffer::Create();

        int cubeMapSize = 512;

        m_GeometryBuffer_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_FrameRenderBuffer_skybox_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_FrameRenderBuffer_DirectLighting_diffuse_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_FrameRenderBuffer_DirectLighting_specular_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_FrameRenderBuffer_EnvironmentLighting_diffuse_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_FrameRenderBuffer_EnvironmentLighting_specular_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_SSAOBuffer_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_FrameRenderBuffer_ssr_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_FrameRenderBuffer_ssr_blur_Baking->SetViewPort(cubeMapSize, cubeMapSize);
        m_FrameRenderBuffer_Baking->SetViewPort(cubeMapSize, cubeMapSize);

        // set point light cube shadow map viewport
        for (auto [entity, name, trans, light] : pointlight_view.each())
        {
            light.SetShadowCubeMapViewPort(512, 512);
        }

        // set directional light shadow map viewport
        for (auto [entity, name, trans, light] : dirctionallight_view.each())
        {
            light.SetShadowMapViewPort(3072, 3072);
        }

        // Get Main SkyBox
        for (auto [entity, name, trans, skybox] : skybox_view.each())
        {
            m_MainSkybox = static_cast<ASkybox*>(skybox.GetOwner());
        }

        // Render to point light ShadowCubeMap
        for (auto [entity, name, trans, light] : pointlight_view.each())
        {
            ShadowCubeMapBuffer&                 shadowCubeMapBuffer = light.GetShadowCubeMapBufferRef();
            std::array<std::function<void()>, 6> renderFuncs {
                std::bind(&ShadowCubeMapBuffer::BindTop, &shadowCubeMapBuffer),
                std::bind(&ShadowCubeMapBuffer::BindBottom, &shadowCubeMapBuffer),
                std::bind(&ShadowCubeMapBuffer::BindLeft, &shadowCubeMapBuffer),
                std::bind(&ShadowCubeMapBuffer::BindRight, &shadowCubeMapBuffer),
                std::bind(&ShadowCubeMapBuffer::BindFront, &shadowCubeMapBuffer),
                std::bind(&ShadowCubeMapBuffer::BindBack, &shadowCubeMapBuffer),
            };

            glm::vec3                lightPosition      = trans.GetPosition();
            glm::mat4                projectionMatrices = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 10.0f);
            std::array<glm::mat4, 6> viewMatrices       = {
                glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            };

            for (int i = 0; i < 6; ++i)
            {
                const std::function<void()>& bindFunc = renderFuncs[i];
                bindFunc();

                RenderCommand::SetViewPort(0, 0, 512, 512);
                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                // use a range-for
                for (auto [entity, name, trans, model] : model_view.each())
                {
                    const auto meshes = model.GetStaticMesh().m_Meshes;
                    for (const auto& mesh : meshes)
                    {
                        auto shader = m_ShaderLibrary.Get("ShadowMap");
                        shader->Bind();

                        shader->SetMat4("u_MLightSpace", projectionMatrices * viewMatrices[i]);
                        shader->SetMat4("u_MTransform", trans.GetTransform());

                        mesh->m_VertexArray->Bind();
                        RenderCommand::DrawIndexed(mesh->m_VertexArray);
                        mesh->m_VertexArray->UnBind();

                        shader->UnBind();
                    }
                }

                shadowCubeMapBuffer.UnBind();
            }
        }

        // Render to directional light ShadowMap
        for (auto [entity, name, trans, light] : dirctionallight_view.each())
        {
            ShadowMapBuffer& shadowMapBuffer = light.GetShadowMapBufferRef();
            shadowMapBuffer.Bind();

            RenderCommand::SetViewPort(0, 0, 3072, 3072);
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            const glm::mat4 lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.5f, 120.0f);
            const glm::vec3 lightPosition   = trans.GetPosition();
            const glm::vec3 lightDirection  = light.GetDirectionRef();
            const glm::mat4 lightView =
                glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
            const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

            // use a range-for
            for (auto [entity, name, trans, model] : model_view.each())
            {
                const auto meshes = model.GetStaticMesh().m_Meshes;
                for (const auto& mesh : meshes)
                {
                    auto shader = m_ShaderLibrary.Get("ShadowMap");
                    shader->Bind();

                    shader->SetMat4("u_MLightSpace", lightSpaceMatrix);
                    shader->SetMat4("u_MTransform", trans.GetTransform());

                    mesh->m_VertexArray->Bind();
                    RenderCommand::DrawIndexed(mesh->m_VertexArray);
                    mesh->m_VertexArray->UnBind();

                    shader->UnBind();
                }
            }

            shadowMapBuffer.UnBind();
        }

        glm::vec3 probePosition = glm::vec3(-9.0f, 5.0f, 3.0f);

        glm::mat4                projectionMatrices = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
        std::array<glm::mat4, 6> viewMatrices       = {
            glm::lookAt(probePosition, probePosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(probePosition, probePosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(probePosition, probePosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            glm::lookAt(probePosition, probePosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            glm::lookAt(probePosition, probePosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(probePosition, probePosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        };

        m_Probe_CubeMap = CubeMap::Create();
        typedef std::array<std::array<std::array<float, 3>, 512>, 512> cube_image;
        std::vector<cube_image>                                        cubeMapData(6);

        for (int i = 0; i < 6; ++i)
        {
            // Render to GeometryBuffer
            m_VMatrix  = viewMatrices[i];
            m_PMatrix  = projectionMatrices;
            m_VPMatrix = m_PMatrix * m_VMatrix;

            m_GeometryBuffer_Baking->Bind();
            RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            // use a range-for
            for (auto [entity, name, trans, model] : model_view.each())
            {
                AStaticMesh* staticMesh_actor = static_cast<AStaticMesh*>(model.GetOwner());
                MMaterial*   material         = static_cast<MMaterial*>(staticMesh_actor->GetMaterial());
                std::string  materialType     = material->GetMaterialType();

                if (staticMesh_actor->GetVisible() == false)
                {
                    continue;
                }

                if (materialType == "BasicPbr")
                {
                    auto shader = m_ShaderLibrary.Get("GBuffer");
                    shader->Bind();

                    shader->SetMat4("u_MProjection", m_PMatrix);
                    shader->SetMat4("u_MView", m_VMatrix);
                    shader->SetMat4("u_MTransform", trans.GetTransform());

                    const auto& materials = model.GetStaticMesh().m_LoadedMaterials;
                    const auto& meshes    = model.GetStaticMesh().m_LoadedMeshes;

                    for (const auto& meshArray : meshes)
                    {
                        std::string materialName      = meshArray.first;
                        const auto  material_basicPbr = static_cast<MBasicPbr*>(materials.at(materialName));
                        material_basicPbr->BindAllMap(shader);

                        uint32_t drawedMeshCount = 0;
                        for (const auto& mesh : meshArray.second)
                        {
                            const FrustumVolume* frustumVolume = mesh->GetFrustumVolume();

                            Renderer::Submit(mesh->m_VertexArray, shader, m_VPMatrix, trans.GetTransform());
                            drawedMeshCount++;
                        }
                        material_basicPbr->UnBindAllMap(shader);
                    }
                    shader->UnBind();
                }
            }

            m_GeometryBuffer_Baking->UnBind();

            // Render to SSAOBuffer

            m_VMatrix  = viewMatrices[i];
            m_PMatrix  = projectionMatrices;
            m_VPMatrix = m_PMatrix * m_VMatrix;

            m_SSAOBuffer_Baking->Bind();
            RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto computeAO_shader = m_ShaderLibrary.Get("ComputeAO");
            computeAO_shader->Bind();

            computeAO_shader->SetInt("g_ViewPosition", 0);
            m_GeometryBuffer_Baking->BindViewPositionTexture(0);
            computeAO_shader->SetInt("g_ViewNormal", 1);
            m_GeometryBuffer_Baking->BindViewNormalTexture(1);
            computeAO_shader->SetInt("g_Depth", 2);
            m_GeometryBuffer_Baking->BindDepthTexture(2);

            computeAO_shader->SetInt("texNoise", 3);
            m_SSAOBuffer_Baking->BindNoiseTexture(3);

            std::vector<glm::vec3>& kernel = m_SSAOBuffer_Baking->GetSSAOKernel();
            for (unsigned int i = 0; i < kernel.size(); ++i)
            {
                computeAO_shader->SetFloat3("samples[" + std::to_string(i) + "]", kernel[i]);
            }
            computeAO_shader->SetMat4("projection", m_PMatrix);

            computeAO_shader->SetFloat("radius", m_SSAOBuffer_Baking->GetRadiusRef());
            computeAO_shader->SetFloat("bias", m_SSAOBuffer_Baking->GetBiasRef());
            computeAO_shader->SetFloat("power", m_SSAOBuffer_Baking->GetPowerRef());

            computeAO_shader->SetFloat("screenWidth", cubeMapSize);
            computeAO_shader->SetFloat("screenHeight", cubeMapSize);

            computeAO_shader->Bind();
            RenderCommand::RenderToQuad();

            m_SSAOBuffer_Baking->UnBindTexture(3);

            m_GeometryBuffer_Baking->UnBindTexture(2);
            m_GeometryBuffer_Baking->UnBindTexture(1);
            m_GeometryBuffer_Baking->UnBindTexture(0);

            computeAO_shader->UnBind();

            m_SSAOBuffer_Baking->UnBind();

            // Render to Skybox

            m_VMatrix  = viewMatrices[i];
            m_PMatrix  = projectionMatrices;
            m_VPMatrix = m_PMatrix * m_VMatrix;

            // skybox shading
            {
                m_FrameRenderBuffer_skybox_Baking->Bind();
                RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);

                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                if (m_RenderSkybox)
                {
                    // draw skybox
                    // use a range-for
                    for (auto [entity, name, trans, skybox] : skybox_view.each())
                    {
                        auto shader = m_ShaderLibrary.Get("Skybox");
                        Renderer::SetShaderUniform(shader, "mipLevel", m_VisPrePrefilterMipLevel);

                        auto vpmat = m_PMatrix * glm::mat4(glm::mat3(m_VMatrix));
                        skybox.Draw(shader, vpmat);
                    }
                }

                m_FrameRenderBuffer_skybox_Baking->UnBind();
            }

            // DirectLighting_diffuse Shading
            {
                m_FrameRenderBuffer_DirectLighting_diffuse_Baking->Bind();
                RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);

                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                auto deferred_shader = m_ShaderLibrary.Get("DirectLighting_diffuse");
                deferred_shader->Bind();

                deferred_shader->SetInt("g_ViewPosition", 0);
                m_GeometryBuffer_Baking->BindViewPositionTexture(0);
                deferred_shader->SetInt("g_ViewNormal", 1);
                m_GeometryBuffer_Baking->BindViewNormalTexture(1);
                deferred_shader->SetInt("g_Diffuse", 2);
                m_GeometryBuffer_Baking->BindDiffuseTexture(2);
                deferred_shader->SetInt("g_Depth", 3);
                m_GeometryBuffer_Baking->BindDepthTexture(3);
                deferred_shader->SetInt("g_Roughness", 4);
                m_GeometryBuffer_Baking->BindRoughnessTexture(4);
                deferred_shader->SetInt("g_Specular", 5);
                m_GeometryBuffer_Baking->BindSpecularTexture(5);
                deferred_shader->SetInt("g_WorldPosition", 6);
                m_GeometryBuffer_Baking->BindWorldPositionTexture(6);
                deferred_shader->SetInt("g_WorldNormal", 7);
                m_GeometryBuffer_Baking->BindWorldNormalTexture(7);
                deferred_shader->SetInt("g_AO", 8);
                m_SSAOBuffer_Baking->BindSSAOTexture(8);

                int ligth_num = 0;
                for (auto [entity, name, trans, light] : pointlight_view.each())
                {
                    deferred_shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                               trans.GetPosition());
                    deferred_shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]",
                                               light.GetColorRef());
                    deferred_shader->SetFloat("pointLightIntensities[" + std::to_string(ligth_num) + "]",
                                              light.GetIntensityRef());

                    deferred_shader->SetInt("pointShadowCubeMaps[" + std::to_string(ligth_num) + "]", 50 + ligth_num);
                    light.GetShadowCubeMapBufferRef().BindTexture(50 + ligth_num);
                    deferred_shader->SetFloat("pointShadowCubeMapsNearPlane[" + std::to_string(ligth_num) + "]", 1.0f);
                    deferred_shader->SetFloat("pointShadowCubeMapsFarPlane[" + std::to_string(ligth_num) + "]", 10.0f);

                    ligth_num++;
                }
                deferred_shader->SetInt("numPointLights", ligth_num);

                int dirctionallight_num = 0;
                for (auto [entity, name, trans, light] : dirctionallight_view.each())
                {
                    const glm::mat4 lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.5f, 120.0f);
                    const glm::vec3 lightPosition   = trans.GetPosition();
                    const glm::vec3 lightDirection  = light.GetDirectionRef();
                    const glm::mat4 lightView =
                        glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
                    const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

                    deferred_shader->SetFloat3("directionalLightDirections[" + std::to_string(dirctionallight_num) +
                                                   "]",
                                               light.GetDirectionRef());
                    deferred_shader->SetFloat3("directionalLightColors[" + std::to_string(dirctionallight_num) + "]",
                                               light.GetColorRef());
                    deferred_shader->SetFloat("directionalLightIntensities[" + std::to_string(dirctionallight_num) +
                                                  "]",
                                              light.GetIntensityRef());
                    deferred_shader->SetMat4("directionalLightMatrices[" + std::to_string(dirctionallight_num) + "]",
                                             lightSpaceMatrix);
                    deferred_shader->SetInt("directionalShadowMaps[" + std::to_string(dirctionallight_num) + "]",
                                            150 + dirctionallight_num);

                    light.GetShadowMapBufferRef().BindTexture(150 + dirctionallight_num);

                    dirctionallight_num++;
                }
                deferred_shader->SetInt("numDirectionalLights", dirctionallight_num);
                deferred_shader->SetFloat("PCSS_FilterRadius", m_PCSS_FilterRadius);

                glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                deferred_shader->SetFloat3("camPos", camPos);

                auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

                deferred_shader->SetInt("irradianceMap", 15);
                sky_cubeMap->BindIrradianceTexture(15);
                deferred_shader->SetInt("prefilterMap", 16);
                sky_cubeMap->BindPrefilterTexture(16);
                deferred_shader->SetInt("brdfLUT", 17);
                sky_cubeMap->BindBrdfLutTexture(17);

                RenderCommand::RenderToQuad();

                sky_cubeMap->UnBind(17);
                sky_cubeMap->UnBind(16);
                sky_cubeMap->UnBind(15);

                m_SSAOBuffer_Baking->UnBindTexture(8);
                m_GeometryBuffer_Baking->UnBindTexture(7);
                m_GeometryBuffer_Baking->UnBindTexture(6);
                m_GeometryBuffer_Baking->UnBindTexture(5);
                m_GeometryBuffer_Baking->UnBindTexture(4);
                m_GeometryBuffer_Baking->UnBindTexture(3);
                m_GeometryBuffer_Baking->UnBindTexture(2);
                m_GeometryBuffer_Baking->UnBindTexture(1);
                m_GeometryBuffer_Baking->UnBindTexture(0);

                deferred_shader->UnBind();
                m_FrameRenderBuffer_DirectLighting_diffuse_Baking->UnBind();
            }
            // DirectLighting_specular Shading
            {
                m_FrameRenderBuffer_DirectLighting_specular_Baking->Bind();
                RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);

                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                auto deferred_shader = m_ShaderLibrary.Get("DirectLighting_specular");
                deferred_shader->Bind();

                deferred_shader->SetInt("g_ViewPosition", 0);
                m_GeometryBuffer_Baking->BindViewPositionTexture(0);
                deferred_shader->SetInt("g_ViewNormal", 1);
                m_GeometryBuffer_Baking->BindViewNormalTexture(1);
                deferred_shader->SetInt("g_Diffuse", 2);
                m_GeometryBuffer_Baking->BindDiffuseTexture(2);
                deferred_shader->SetInt("g_Depth", 3);
                m_GeometryBuffer_Baking->BindDepthTexture(3);
                deferred_shader->SetInt("g_Roughness", 4);
                m_GeometryBuffer_Baking->BindRoughnessTexture(4);
                deferred_shader->SetInt("g_Specular", 5);
                m_GeometryBuffer_Baking->BindSpecularTexture(5);
                deferred_shader->SetInt("g_WorldPosition", 6);
                m_GeometryBuffer_Baking->BindWorldPositionTexture(6);
                deferred_shader->SetInt("g_WorldNormal", 7);
                m_GeometryBuffer_Baking->BindWorldNormalTexture(7);
                deferred_shader->SetInt("g_AO", 8);
                m_SSAOBuffer_Baking->BindSSAOTexture(8);

                int ligth_num = 0;
                for (auto [entity, name, trans, light] : pointlight_view.each())
                {
                    deferred_shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                               trans.GetPosition());
                    deferred_shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]",
                                               light.GetColorRef());
                    deferred_shader->SetFloat("pointLightIntensities[" + std::to_string(ligth_num) + "]",
                                              light.GetIntensityRef());

                    deferred_shader->SetInt("pointShadowCubeMaps[" + std::to_string(ligth_num) + "]", 50 + ligth_num);
                    light.GetShadowCubeMapBufferRef().BindTexture(50 + ligth_num);
                    deferred_shader->SetFloat("pointShadowCubeMapsNearPlane[" + std::to_string(ligth_num) + "]", 1.0f);
                    deferred_shader->SetFloat("pointShadowCubeMapsFarPlane[" + std::to_string(ligth_num) + "]", 10.0f);

                    ligth_num++;
                }
                deferred_shader->SetInt("numPointLights", ligth_num);

                int dirctionallight_num = 0;
                for (auto [entity, name, trans, light] : dirctionallight_view.each())
                {
                    const glm::mat4 lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.5f, 120.0f);
                    const glm::vec3 lightPosition   = trans.GetPosition();
                    const glm::vec3 lightDirection  = light.GetDirectionRef();
                    const glm::mat4 lightView =
                        glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
                    const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

                    deferred_shader->SetFloat3("directionalLightDirections[" + std::to_string(dirctionallight_num) +
                                                   "]",
                                               light.GetDirectionRef());
                    deferred_shader->SetFloat3("directionalLightColors[" + std::to_string(dirctionallight_num) + "]",
                                               light.GetColorRef());
                    deferred_shader->SetFloat("directionalLightIntensities[" + std::to_string(dirctionallight_num) +
                                                  "]",
                                              light.GetIntensityRef());
                    deferred_shader->SetMat4("directionalLightMatrices[" + std::to_string(dirctionallight_num) + "]",
                                             lightSpaceMatrix);
                    deferred_shader->SetInt("directionalShadowMaps[" + std::to_string(dirctionallight_num) + "]",
                                            150 + dirctionallight_num);

                    light.GetShadowMapBufferRef().BindTexture(150 + dirctionallight_num);

                    dirctionallight_num++;
                }
                deferred_shader->SetInt("numDirectionalLights", dirctionallight_num);
                deferred_shader->SetFloat("PCSS_FilterRadius", m_PCSS_FilterRadius);

                glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                deferred_shader->SetFloat3("camPos", camPos);

                auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

                deferred_shader->SetInt("irradianceMap", 15);
                sky_cubeMap->BindIrradianceTexture(15);
                deferred_shader->SetInt("prefilterMap", 16);
                sky_cubeMap->BindPrefilterTexture(16);
                deferred_shader->SetInt("brdfLUT", 17);
                sky_cubeMap->BindBrdfLutTexture(17);

                RenderCommand::RenderToQuad();

                sky_cubeMap->UnBind(17);
                sky_cubeMap->UnBind(16);
                sky_cubeMap->UnBind(15);

                m_SSAOBuffer_Baking->UnBindTexture(8);
                m_GeometryBuffer_Baking->UnBindTexture(7);
                m_GeometryBuffer_Baking->UnBindTexture(6);
                m_GeometryBuffer_Baking->UnBindTexture(5);
                m_GeometryBuffer_Baking->UnBindTexture(4);
                m_GeometryBuffer_Baking->UnBindTexture(3);
                m_GeometryBuffer_Baking->UnBindTexture(2);
                m_GeometryBuffer_Baking->UnBindTexture(1);
                m_GeometryBuffer_Baking->UnBindTexture(0);

                deferred_shader->UnBind();
                m_FrameRenderBuffer_DirectLighting_specular_Baking->UnBind();
            }
            // EnvironmentLighting_diffuse Shading
            {
                m_FrameRenderBuffer_EnvironmentLighting_diffuse_Baking->Bind();
                RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);

                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                auto deferred_shader = m_ShaderLibrary.Get("EnvironmentLighting_diffuse");
                deferred_shader->Bind();

                deferred_shader->SetInt("g_ViewPosition", 0);
                m_GeometryBuffer_Baking->BindViewPositionTexture(0);
                deferred_shader->SetInt("g_ViewNormal", 1);
                m_GeometryBuffer_Baking->BindViewNormalTexture(1);
                deferred_shader->SetInt("g_Diffuse", 2);
                m_GeometryBuffer_Baking->BindDiffuseTexture(2);
                deferred_shader->SetInt("g_Depth", 3);
                m_GeometryBuffer_Baking->BindDepthTexture(3);
                deferred_shader->SetInt("g_Roughness", 4);
                m_GeometryBuffer_Baking->BindRoughnessTexture(4);
                deferred_shader->SetInt("g_Specular", 5);
                m_GeometryBuffer_Baking->BindSpecularTexture(5);
                deferred_shader->SetInt("g_WorldPosition", 6);
                m_GeometryBuffer_Baking->BindWorldPositionTexture(6);
                deferred_shader->SetInt("g_WorldNormal", 7);
                m_GeometryBuffer_Baking->BindWorldNormalTexture(7);
                deferred_shader->SetInt("g_AO", 8);
                m_SSAOBuffer_Baking->BindSSAOTexture(8);

                int ligth_num = 0;
                for (auto [entity, name, trans, light] : pointlight_view.each())
                {
                    deferred_shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                               trans.GetPosition());
                    deferred_shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]",
                                               light.GetColorRef());

                    ligth_num++;
                }
                deferred_shader->SetInt("numPointLights", ligth_num);

                glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                deferred_shader->SetFloat3("camPos", camPos);

                auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

                const auto& sphericlaHarmonicsParameters = sky_cubeMap->GetSphereHarmonicsParametersRef();
                for (int i = 0; i < 9; ++i)
                {
                    deferred_shader->SetFloat3("sphericlaHarmonicsParameters[" + std::to_string(i) + "]",
                                               sphericlaHarmonicsParameters[i]);
                }

                deferred_shader->SetInt("prefilterMap", 16);
                sky_cubeMap->BindPrefilterTexture(16);
                deferred_shader->SetInt("brdfLUT", 17);
                sky_cubeMap->BindBrdfLutTexture(17);

                RenderCommand::RenderToQuad();

                sky_cubeMap->UnBind(17);
                sky_cubeMap->UnBind(16);
                sky_cubeMap->UnBind(15);

                m_SSAOBuffer_Baking->UnBindTexture(8);
                m_GeometryBuffer_Baking->UnBindTexture(7);
                m_GeometryBuffer_Baking->UnBindTexture(6);
                m_GeometryBuffer_Baking->UnBindTexture(5);
                m_GeometryBuffer_Baking->UnBindTexture(4);
                m_GeometryBuffer_Baking->UnBindTexture(3);
                m_GeometryBuffer_Baking->UnBindTexture(2);
                m_GeometryBuffer_Baking->UnBindTexture(1);
                m_GeometryBuffer_Baking->UnBindTexture(0);

                deferred_shader->UnBind();
                m_FrameRenderBuffer_EnvironmentLighting_diffuse_Baking->UnBind();
            }
            // EnvironmentLighting_specular Shading
            {
                m_FrameRenderBuffer_EnvironmentLighting_specular_Baking->Bind();
                RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);

                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                auto deferred_shader = m_ShaderLibrary.Get("EnvironmentLighting_specular");
                deferred_shader->Bind();

                deferred_shader->SetInt("g_ViewPosition", 0);
                m_GeometryBuffer_Baking->BindViewPositionTexture(0);
                deferred_shader->SetInt("g_ViewNormal", 1);
                m_GeometryBuffer_Baking->BindViewNormalTexture(1);
                deferred_shader->SetInt("g_Diffuse", 2);
                m_GeometryBuffer_Baking->BindDiffuseTexture(2);
                deferred_shader->SetInt("g_Depth", 3);
                m_GeometryBuffer_Baking->BindDepthTexture(3);
                deferred_shader->SetInt("g_Roughness", 4);
                m_GeometryBuffer_Baking->BindRoughnessTexture(4);
                deferred_shader->SetInt("g_Specular", 5);
                m_GeometryBuffer_Baking->BindSpecularTexture(5);
                deferred_shader->SetInt("g_WorldPosition", 6);
                m_GeometryBuffer_Baking->BindWorldPositionTexture(6);
                deferred_shader->SetInt("g_WorldNormal", 7);
                m_GeometryBuffer_Baking->BindWorldNormalTexture(7);
                deferred_shader->SetInt("g_AO", 8);
                m_SSAOBuffer->BindSSAOTexture(8);

                int ligth_num = 0;
                for (auto [entity, name, trans, light] : pointlight_view.each())
                {
                    deferred_shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                               trans.GetPosition());
                    deferred_shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]",
                                               light.GetColorRef());

                    ligth_num++;
                }
                deferred_shader->SetInt("numPointLights", ligth_num);

                glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                deferred_shader->SetFloat3("camPos", camPos);

                auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

                deferred_shader->SetInt("irradianceMap", 15);
                sky_cubeMap->BindIrradianceTexture(15);
                deferred_shader->SetInt("prefilterMap", 16);
                m_Probe_CubeMap->BindPrefilterTexture(16);
                deferred_shader->SetInt("brdfLUT", 17);
                sky_cubeMap->BindBrdfLutTexture(17);

                RenderCommand::RenderToQuad();

                sky_cubeMap->UnBind(17);
                sky_cubeMap->UnBind(16);
                sky_cubeMap->UnBind(15);

                m_SSAOBuffer->UnBindTexture(8);
                m_GeometryBuffer->UnBindTexture(7);
                m_GeometryBuffer->UnBindTexture(6);
                m_GeometryBuffer->UnBindTexture(5);
                m_GeometryBuffer->UnBindTexture(4);
                m_GeometryBuffer->UnBindTexture(3);
                m_GeometryBuffer->UnBindTexture(2);
                m_GeometryBuffer->UnBindTexture(1);
                m_GeometryBuffer->UnBindTexture(0);

                deferred_shader->UnBind();
                m_FrameRenderBuffer_EnvironmentLighting_specular_Baking->UnBind();
            }

            // ssr
            {
                glDisable(GL_DEPTH_TEST);
                m_FrameRenderBuffer_ssr_Baking->Bind();
                RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);
                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                // auto ssr_shader = m_ShaderLibrary.Get("ScreenSpaceReflection");
                // ssr_shader->Bind();

                // ssr_shader->SetInt("g_DirectLightinging_diffuse", 0);
                // m_FrameRenderBuffer_DirectLighting_diffuse_Baking->BindTexture(0);
                // ssr_shader->SetInt("g_DirectLightinging_specular", 1);
                // m_FrameRenderBuffer_DirectLighting_specular_Baking->BindTexture(1);
                // ssr_shader->SetInt("g_EnvironmentLighting_diffuse", 2);
                // m_FrameRenderBuffer_EnvironmentLighting_diffuse_Baking->BindTexture(2);
                // ssr_shader->SetInt("g_EnvironmentLighting_specular", 3);
                // m_FrameRenderBuffer_EnvironmentLighting_specular_Baking->BindTexture(3);
                // ssr_shader->SetInt("g_ViewPosition", 4);
                // m_GeometryBuffer_Baking->BindViewPositionTexture(4);
                // ssr_shader->SetInt("g_ViewNormal", 5);
                // m_GeometryBuffer_Baking->BindViewNormalTexture(5);
                // ssr_shader->SetInt("g_Depth", 6);
                // m_GeometryBuffer_Baking->BindDepthTexture(6);
                // ssr_shader->SetInt("g_Specular", 7);
                // m_GeometryBuffer_Baking->BindSpecularTexture(7);
                // ssr_shader->SetInt("g_WorldPosition", 8);
                // m_GeometryBuffer_Baking->BindWorldPositionTexture(8);
                // ssr_shader->SetInt("g_Roughness", 9);
                // m_GeometryBuffer_Baking->BindRoughnessTexture(9);

                // ssr_shader->SetFloat("rayStep", m_SSR_settings.rayStep);
                // ssr_shader->SetFloat("minRayStep", m_SSR_settings.minRayStep);
                // ssr_shader->SetFloat("maxSteps", m_SSR_settings.maxSteps);
                // ssr_shader->SetInt("numBinarySearchSteps", m_SSR_settings.numBinarySearchSteps);
                // ssr_shader->SetFloat("reflectionSpecularFalloffExponent",
                //                      m_SSR_settings.reflectionSpecularFalloffExponent);
                // ssr_shader->SetBool("debug", m_SSR_settings.debug);
                // ssr_shader->SetFloat("refBias", m_SSR_settings.refBias);

                // ssr_shader->SetMat4("u_MProjection", m_PMatrix);
                // ssr_shader->SetMat4("u_MView", m_VMatrix);
                // ssr_shader->SetMat4("u_MInvProjection", glm::inverse(m_PMatrix));
                // ssr_shader->SetMat4("u_MInvView", glm::inverse(m_VMatrix));

                // RenderCommand::RenderToQuad();

                // m_GeometryBuffer_Baking->UnBindTexture(9);
                // m_GeometryBuffer_Baking->UnBindTexture(8);
                // m_GeometryBuffer_Baking->UnBindTexture(7);
                // m_GeometryBuffer_Baking->UnBindTexture(6);
                // m_GeometryBuffer_Baking->UnBindTexture(5);
                // m_GeometryBuffer_Baking->UnBindTexture(4);
                // m_FrameRenderBuffer_EnvironmentLighting_specular_Baking->UnBindTexture(3);
                // m_FrameRenderBuffer_EnvironmentLighting_diffuse_Baking->UnBindTexture(2);
                // m_FrameRenderBuffer_DirectLighting_specular_Baking->UnBindTexture(1);
                // m_FrameRenderBuffer_DirectLighting_diffuse_Baking->UnBindTexture(0);

                // ssr_shader->UnBind();
                Renderer::EndScene(m_FrameRenderBuffer_ssr_Baking);
                glEnable(GL_DEPTH_TEST);
            }
            // blur ssr
            {
                glDisable(GL_DEPTH_TEST);
                m_FrameRenderBuffer_ssr_blur_Baking->Bind();
                RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);
                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                // auto blur_shader = m_ShaderLibrary.Get("GaussianBlur");
                // blur_shader->Bind();

                // blur_shader->SetInt("g_Color", 0);
                // m_FrameRenderBuffer_ssr_Baking->BindTexture(0);
                // blur_shader->SetFloat2("screenSize", glm::vec2(cubeMapSize, cubeMapSize) / 1.0f);

                // RenderCommand::RenderToQuad();

                // m_FrameRenderBuffer_ssr_Baking->UnBindTexture(0);

                // blur_shader->UnBind();
                Renderer::EndScene(m_FrameRenderBuffer_ssr_blur_Baking);
                glEnable(GL_DEPTH_TEST);
            }

            // composite
            {
                glDisable(GL_DEPTH_TEST);
                m_FrameRenderBuffer_Baking->Bind();
                RenderCommand::SetViewPort(0, 0, cubeMapSize, cubeMapSize);
                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                auto composite_shader = m_ShaderLibrary.Get("Composite");
                composite_shader->Bind();

                composite_shader->SetInt("g_DirectLightinging_diffuse", 0);
                m_FrameRenderBuffer_DirectLighting_diffuse_Baking->BindTexture(0);
                composite_shader->SetInt("g_DirectLightinging_specular", 1);
                m_FrameRenderBuffer_DirectLighting_specular_Baking->BindTexture(1);
                composite_shader->SetInt("g_EnvironmentLighting_diffuse", 2);
                m_FrameRenderBuffer_EnvironmentLighting_diffuse_Baking->BindTexture(2);
                composite_shader->SetInt("g_EnvironmentLighting_specular", 3);
                m_FrameRenderBuffer_EnvironmentLighting_specular_Baking->BindTexture(3);
                composite_shader->SetInt("g_Skybox", 4);
                m_FrameRenderBuffer_skybox_Baking->BindTexture(4);
                composite_shader->SetInt("g_ScreenSpaceReflection", 5);
                m_FrameRenderBuffer_ssr_blur_Baking->BindTexture(5);
                composite_shader->SetInt("g_AO", 6);
                m_SSAOBuffer_Baking->BindSSAOTexture(6);

                RenderCommand::RenderToQuad();

                m_SSAOBuffer_Baking->UnBindTexture(6);
                m_FrameRenderBuffer_ssr_blur_Baking->UnBindTexture(5);
                m_FrameRenderBuffer_skybox_Baking->UnBindTexture(4);
                m_FrameRenderBuffer_EnvironmentLighting_specular_Baking->UnBindTexture(3);
                m_FrameRenderBuffer_EnvironmentLighting_diffuse_Baking->UnBindTexture(2);
                m_FrameRenderBuffer_DirectLighting_specular_Baking->UnBindTexture(1);
                m_FrameRenderBuffer_DirectLighting_diffuse_Baking->UnBindTexture(0);

                composite_shader->UnBind();
                Renderer::EndScene(m_FrameRenderBuffer_Baking);
                glEnable(GL_DEPTH_TEST);
            }

            {
                // copy to cubeMapData
                glBindTexture(GL_TEXTURE_2D, (GLuint)(size_t)m_FrameRenderBuffer_Baking->GetTextureID());
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, cubeMapData[i].data());
                glBindTexture(GL_TEXTURE_2D, 0);

                glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)(size_t)m_Probe_CubeMap->GetTextureID());
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,
                             GL_RGB16F,
                             cubeMapSize,
                             cubeMapSize,
                             0,
                             GL_RGB,
                             GL_FLOAT,
                             cubeMapData[i].data());
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            }
        }

        m_Probe_CubeMap->ComputeIrradianceTexture();
        m_Probe_CubeMap->ComputeSphereHarmonicsParameters();
        m_Probe_CubeMap->ComputePrefilterTexture();

        sphereHarmonicsParameters_a = m_Probe_CubeMap->GetSphereHarmonicsParametersRef();
    }

    void UWorld::TickLogic(float timeStep, float nowTime, bool isWindowFocused)
    {
        // Render
        float m_CameraTranslationSpeed      = 7.0;
        float m_CameraTranslationSpeedMouse = 0.5;
        float m_CameraRotationSpeed         = 0.15;

        auto camrea_view          = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto pawn_view            = m_Registry.view<UTagComponent, UTransformComponent, UPawnComponent>();
        auto skeleton_view        = m_Registry.view<UTagComponent, UTransformComponent, USkeletonComponent>();
        auto trajectory_view      = m_Registry.view<UTagComponent, UTransformComponent, UTrajectoryComponent>();
        auto pointlight_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();
        auto skinnedmesh_view     = m_Registry.view<UTagComponent, UTransformComponent, USkinnedMeshComponent>();
        auto motionmatching_view  = m_Registry.view<UTagComponent, UTransformComponent, UMotionMatchingComponent>();
        auto chunkarrayworld_view = m_Registry.view<UTagComponent, UTransformComponent, UChunkArrayWorldComponent>();

        auto [currentX, currentY] = Input::GetMousePostion();

        static float lastX;
        static float lastY;

        float deltaX = currentX - lastX;
        float deltaY = currentY - lastY;

        lastX = currentX;
        lastY = currentY;

        static enum class InteractionMode {
            Editor,
            FirstPerson,
        } interactionMode = InteractionMode::Editor;

        // use a range-for
        for (auto [entity, name, trans, camera] : camrea_view.each())
        {
            if (m_Registry.any_of<UPawnComponent>(entity))
            {
                continue;
            }

            AActor*  actor   = static_cast<AActor*>(camera.GetOwner());
            ACamera* aCamera = static_cast<ACamera*>(actor);

            if (Input::IsKeyPressed(GLFW_KEY_F))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_None);
                ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
                glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                interactionMode = InteractionMode::FirstPerson;
                Log::Info("FirstPerson");
            }
            else if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
            {
                glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
                ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

                interactionMode = InteractionMode::Editor;
                Log::Info("Editor");
            }

            const glm::vec3 front =
                -glm::normalize(glm::rotate(glm::quat(trans.GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f)));
            const glm::vec3 right =
                glm::normalize(glm::rotate(glm::quat(trans.GetRotation()), glm::vec3(1.0f, 0.0f, 0.0f)));
            const glm::vec3 up(0.0f, 1.0f, 0.0f);

            if (interactionMode == InteractionMode::Editor)
            {
                if (isWindowFocused && actor->GetIsControlled() && aCamera->GetIsViewportCamera())
                {

                    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) &&
                        Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
                    {
                        auto newPosition = trans.GetPosition();
                        newPosition -= deltaY * glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) *
                                       m_CameraTranslationSpeedMouse * timeStep;
                        newPosition += deltaX * glm::normalize(glm::vec3(right[0], 0.0f, right[2])) *
                                       m_CameraTranslationSpeedMouse * timeStep;

                        trans.SetPosition(newPosition);
                    }
                    else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
                    {
                        auto newRotation = trans.GetRotation();

                        newRotation.y -= deltaX * m_CameraRotationSpeed * timeStep;
                        newRotation.x -= deltaY * m_CameraRotationSpeed * timeStep;

                        trans.SetRotation(newRotation);
                    }
                }
            }
            else if (interactionMode == InteractionMode::FirstPerson)
            {
                auto newRotation = trans.GetRotation();

                newRotation.y -= deltaX * m_CameraRotationSpeed * timeStep;
                newRotation.x -= deltaY * m_CameraRotationSpeed * timeStep;

                trans.SetRotation(newRotation);
            }

            if (interactionMode == InteractionMode::FirstPerson || interactionMode == InteractionMode::Editor &&
                                                                       isWindowFocused && actor->GetIsControlled() &&
                                                                       aCamera->GetIsViewportCamera())
            {
                if (Input::IsKeyPressed(GLFW_KEY_A))
                {
                    auto newPosition = trans.GetPosition() - right * m_CameraTranslationSpeed * timeStep;
                    trans.SetPosition(newPosition);
                }
                if (Input::IsKeyPressed(GLFW_KEY_D))
                {
                    auto newPosition = trans.GetPosition() + right * m_CameraTranslationSpeed * timeStep;
                    trans.SetPosition(newPosition);
                }
                if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
                {
                    auto newPosition = trans.GetPosition() - up * m_CameraTranslationSpeed * timeStep;
                    trans.SetPosition(newPosition);
                }
                if (Input::IsKeyPressed(GLFW_KEY_SPACE))
                {
                    auto newPosition = trans.GetPosition() + up * m_CameraTranslationSpeed * timeStep;
                    trans.SetPosition(newPosition);
                }
                if (Input::IsKeyPressed(GLFW_KEY_W))
                {
                    auto newPosition = trans.GetPosition() + front * m_CameraTranslationSpeed * timeStep;
                    trans.SetPosition(newPosition);
                }
                if (Input::IsKeyPressed(GLFW_KEY_S))
                {
                    auto newPosition = trans.GetPosition() - front * m_CameraTranslationSpeed * timeStep;
                    trans.SetPosition(newPosition);
                }
            }
        }

        AActor*   actor_mainCamera = static_cast<AActor*>(m_MainCamera->GetOwner());
        glm::mat4 cameraTransform =
            actor_mainCamera->GetTransformComponent().GetTransform() * m_MainCamera->GetTransform();
        glm::vec3 cameraPosition    = actor_mainCamera->GetTransformComponent().GetPosition();
        glm::vec3 cameraFrontVector = -glm::normalize(glm::rotate(
            glm::quat(actor_mainCamera->GetTransformComponent().GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f)));

        // Update Trajectory
        for (auto [entity, name, trans, trajectory] : trajectory_view.each())
        {
            trajectory.TickLogic(timeStep, cameraTransform);
        }

        // Update Skeleton
        for (auto [entity, name, trans, skeleton] : skeleton_view.each())
        {
            skeleton.Update(fmod(0.06 * nowTime, 1.0f));
        }

        // Update SkinnedMesh
        for (auto [entity, name, trans, skinnedmesh] : skinnedmesh_view.each())
        {
            skinnedmesh.Update(fmod(0.0015 * nowTime, 1.0f));
        }

        // Update Pawn

        for (auto [entity, name, trans, pawn] : pawn_view.each())
        {
            pawn.TickLogic(timeStep);
        }

        // Update MotionMatching
        for (auto [entity, name, trans, motionmatching] : motionmatching_view.each())
        {
            motionmatching.Update0(nowTime, timeStep);
        }

        // Update ChunkWorld
        for (auto [entity, name, trans, world] : chunkarrayworld_view.each())
        {
            world.TickLogic(timeStep);

            static bool  button01Released = true;
            static bool  button02Released = true;
            static float lastClickTime    = 0.0f;
            if (Input::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1))
            {
                button01Released = true;
            }
            if (Input::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_2))
            {
                button02Released = true;
            }

            if (interactionMode == InteractionMode::FirstPerson)
            {
                if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
                {
                    if (nowTime - lastClickTime > 0.1f && button01Released)
                    {
                        std::vector<UChunkArrayWorldComponent::IntersectResult> intersectedVoxelArray =
                            world.IntersectRay(cameraPosition, cameraFrontVector, 10.0f);
                        if (intersectedVoxelArray.size() > 0)
                        {
                            world.ExcavateVoxel(intersectedVoxelArray[0]);

                            lastClickTime    = nowTime;
                            button01Released = false;
                        }
                    }
                }

                if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
                {
                    if (nowTime - lastClickTime > 0.1f && button02Released)
                    {
                        std::vector<UChunkArrayWorldComponent::IntersectResult> intersectedVoxelArray =
                            world.IntersectRay(cameraPosition, cameraFrontVector, 10.0f);
                        if (intersectedVoxelArray.size() > 0)
                        {
                            world.PlaceVoxel(intersectedVoxelArray[0], 3);

                            lastClickTime    = nowTime;
                            button02Released = false;
                        }
                    }
                }
            }
        }
    }

    void UWorld::TickRender(float timeStep)
    {
        // Initialize();
        auto actor_view           = m_Registry.view<UTagComponent, UTransformComponent>();
        auto camrea_view          = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto model_view           = m_Registry.view<UTagComponent, UTransformComponent, UStaticMeshComponent>();
        auto pointcloud_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointCloudComponent>();
        auto pointlight_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();
        auto dirctionallight_view = m_Registry.view<UTagComponent, UTransformComponent, UDirectionalLightComponent>();
        auto skybox_view          = m_Registry.view<UTagComponent, UTransformComponent, USkyboxComponent>();
        auto skeleton_view        = m_Registry.view<UTagComponent, UTransformComponent, USkeletonComponent>();
        auto pawn_view            = m_Registry.view<UTagComponent, UTransformComponent, UPawnComponent>();
        auto trajectory_view      = m_Registry.view<UTagComponent, UTransformComponent, UTrajectoryComponent>();
        auto skinnedmesh_view     = m_Registry.view<UTagComponent, UTransformComponent, USkinnedMeshComponent>();
        auto motionmatching_view  = m_Registry.view<UTagComponent, UTransformComponent, UMotionMatchingComponent>();
        auto chunkarrayworld_view = m_Registry.view<UTagComponent, UTransformComponent, UChunkArrayWorldComponent>();

        m_FrameRenderBuffer_skybox->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_DirectLighting_diffuse->SetViewPort(m_FrameRenderBuffer->GetWidth(),
                                                                m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_DirectLighting_specular->SetViewPort(m_FrameRenderBuffer->GetWidth(),
                                                                 m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_EnvironmentLighting_diffuse->SetViewPort(m_FrameRenderBuffer->GetWidth(),
                                                                     m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_EnvironmentLighting_specular->SetViewPort(m_FrameRenderBuffer->GetWidth(),
                                                                      m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_VoxelGI->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_playground->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

        m_FrameRenderBuffer_ssr->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_ssr_blur->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_exposure->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_GeometryBuffer->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_highLight->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        for (int i = 0; i < 7; ++i)
        {
            m_FrameRenderBuffer_highLight_downSampled[i]->SetViewPort(m_FrameRenderBuffer->GetWidth() / (32 << i),
                                                                      m_FrameRenderBuffer->GetHeight() / (32 << i));
        }

        for (int i = 0; i < 6; ++i)
        {
            m_FrameRenderBuffer_highLight_blur[i]->SetViewPort(m_FrameRenderBuffer->GetWidth() / (32 << i),
                                                               m_FrameRenderBuffer->GetHeight() / (32 << i));
            m_FrameRenderBuffer_highLight_upSampled[i]->SetViewPort(m_FrameRenderBuffer->GetWidth() / (32 << i),
                                                                    m_FrameRenderBuffer->GetHeight() / (32 << i));
        }
        m_FrameRenderBuffer_bloom->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_fxaa->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_VoxelGIViewport->SetViewPort(m_FrameRenderBuffer->GetWidth(),
                                                         m_FrameRenderBuffer->GetHeight());

        m_SSAOBuffer->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_FrameRenderBuffer_shadowMapViewport->SetViewPort(256, 256);
        m_FrameRenderBuffer_shadowCubeMapViewport->SetViewPort(512, 256);

        // set point light cube shadow map viewport
        for (auto [entity, name, trans, light] : pointlight_view.each())
        {
            light.SetShadowCubeMapViewPort(512, 512);
        }

        // set directional light shadow map viewport
        for (auto [entity, name, trans, light] : dirctionallight_view.each())
        {
            light.SetShadowMapViewPort(3072, 3072);
        }

        // Get Main SkyBox
        for (auto [entity, name, trans, skybox] : skybox_view.each())
        {
            m_MainSkybox = static_cast<ASkybox*>(skybox.GetOwner());
        }

        m_MainCamera->GetCamera().SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_MainCamera->GetCamera().RecalculateProjectionMatrix();

        AActor* actor_mainCamera = static_cast<AActor*>(m_MainCamera->GetOwner());

        m_VMatrix =
            glm::inverse(actor_mainCamera->GetTransformComponent().GetTransform() * m_MainCamera->GetTransform());
        m_PMatrix  = m_MainCamera->GetCamera().GetProjectionMatrix();
        m_VPMatrix = m_PMatrix * m_VMatrix;

        // Generate ShadowMap
        static bool isShadowMapGenerated = false;
        if (isShadowMapGenerated == false)
        {
            {
                glCullFace(GL_FRONT);
                // Render to point light ShadowCubeMap
                for (auto [entity, name, trans, light] : pointlight_view.each())
                {
                    ShadowCubeMapBuffer& shadowCubeMapBuffer = light.GetShadowCubeMapBufferRef();
                    // Y+ Y- X- X+ Z+ Z-
                    std::array<std::function<void()>, 6> renderFuncs {
                        std::bind(&ShadowCubeMapBuffer::BindTop, &shadowCubeMapBuffer),
                        std::bind(&ShadowCubeMapBuffer::BindBottom, &shadowCubeMapBuffer),
                        std::bind(&ShadowCubeMapBuffer::BindLeft, &shadowCubeMapBuffer),
                        std::bind(&ShadowCubeMapBuffer::BindRight, &shadowCubeMapBuffer),
                        std::bind(&ShadowCubeMapBuffer::BindFront, &shadowCubeMapBuffer),
                        std::bind(&ShadowCubeMapBuffer::BindBack, &shadowCubeMapBuffer),
                    };

                    glm::vec3 lightPosition               = trans.GetPosition();
                    glm::mat4 projectionMatrices          = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 10.0f);
                    std::array<glm::mat4, 6> viewMatrices = {
                        glm::lookAt(
                            lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                        glm::lookAt(
                            lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                        glm::lookAt(
                            lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                        glm::lookAt(
                            lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                        glm::lookAt(
                            lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                        glm::lookAt(
                            lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                    };

                    for (int i = 0; i < 6; ++i)
                    {
                        const std::function<void()>& bindFunc = renderFuncs[i];
                        bindFunc();

                        RenderCommand::SetViewPort(0, 0, 512, 512);
                        RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                        RenderCommand::Clear();

                        // use a range-for
                        for (auto [entity, name, trans, model] : model_view.each())
                        {
                            const auto meshes = model.GetStaticMesh().m_Meshes;
                            for (const auto& mesh : meshes)
                            {
                                auto shader = m_ShaderLibrary.Get("ShadowMap");
                                shader->Bind();

                                shader->SetMat4("u_MLightSpace", projectionMatrices * viewMatrices[i]);
                                shader->SetMat4("u_MTransform", trans.GetTransform());

                                mesh->m_VertexArray->Bind();
                                RenderCommand::DrawIndexed(mesh->m_VertexArray);
                                mesh->m_VertexArray->UnBind();

                                shader->UnBind();
                            }
                        }

                        // draw chunks
                        for (auto [entity, name, trans, world] : chunkarrayworld_view.each())
                        {
                            auto shader = m_ShaderLibrary.Get("ShadowMap");
                            shader->Bind();

                            for (int x = 0; x < world.GetWorldSizeX(); ++x)
                            {
                                for (int y = 0; y < world.GetWorldSizeY(); ++y)
                                {
                                    for (int z = 0; z < world.GetWorldSizeZ(); ++z)
                                    {
                                        const UChunkArrayWorldComponent::Chunk& chunk = world.GetChunkRef(x, y, z);

                                        glm::vec3 chunkPosition = chunk.position + glm::vec3(16.0f, 16.0f, 16.0f);

                                        if (glm::length(lightPosition - chunkPosition) > 64.0f ||
                                            chunk.component == nullptr)
                                        {
                                            continue;
                                        }

                                        shader->SetMat4("u_MLightSpace", projectionMatrices * viewMatrices[i]);
                                        glm::mat4 model = glm::translate(glm::mat4(1.0f), chunk.position);
                                        shader->SetMat4("u_MTransform", trans.GetTransform() * model);

                                        const VertexArray& vertexArray = chunk.component->GetVertexArrayRef();
                                        vertexArray.Bind();

                                        glDrawArrays(GL_TRIANGLES, 0, chunk.component->GetVertexCount());

                                        vertexArray.UnBind();
                                    }
                                }
                            }

                            shader->UnBind();
                        }

                        shadowCubeMapBuffer.UnBind();
                    }
                }
                glCullFace(GL_BACK);
            }

            // Render to directional light ShadowMap
            {
                glCullFace(GL_FRONT);
                for (auto [entity, name, trans, light] : dirctionallight_view.each())
                {
                    ShadowMapBuffer& shadowMapBuffer = light.GetShadowMapBufferRef();
                    shadowMapBuffer.Bind();

                    RenderCommand::SetViewPort(0, 0, 3072, 3072);
                    RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                    RenderCommand::Clear();

                    const glm::mat4 lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.5f, 120.0f);
                    ;
                    glm::vec3 cameraPosition       = actor_mainCamera->GetTransformComponent().GetPosition();
                    cameraPosition.y               = 0.0f;
                    const glm::vec3 lightPosition  = trans.GetPosition();
                    const glm::vec3 lightDirection = light.GetDirectionRef();
                    const glm::mat4 lightView =
                        glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
                    const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

                    // use a range-for
                    for (auto [entity, name, trans, model] : model_view.each())
                    {
                        const auto meshes = model.GetStaticMesh().m_Meshes;
                        for (const auto& mesh : meshes)
                        {
                            auto shader = m_ShaderLibrary.Get("ShadowMap");
                            shader->Bind();

                            shader->SetMat4("u_MLightSpace", lightSpaceMatrix);
                            shader->SetMat4("u_MTransform", trans.GetTransform());

                            mesh->m_VertexArray->Bind();
                            RenderCommand::DrawIndexed(mesh->m_VertexArray);
                            mesh->m_VertexArray->UnBind();

                            shader->UnBind();
                        }
                    }

                    // draw chunks
                    for (auto [entity, name, trans, world] : chunkarrayworld_view.each())
                    {
                        auto shader = m_ShaderLibrary.Get("ShadowMap");
                        shader->Bind();

                        for (int x = 0; x < world.GetWorldSizeX(); ++x)
                        {
                            for (int y = 0; y < world.GetWorldSizeY(); ++y)
                            {
                                for (int z = 0; z < world.GetWorldSizeZ(); ++z)
                                {
                                    const UChunkArrayWorldComponent::Chunk& chunk = world.GetChunkRef(x, y, z);

                                    glm::vec3 chunkPosition = chunk.position + glm::vec3(16.0f, 16.0f, 16.0f);

                                    if (glm::length(lightPosition - chunkPosition) > 128.0f ||
                                        chunk.component == nullptr)
                                    {
                                        continue;
                                    }

                                    shader->SetMat4("u_MLightSpace", lightSpaceMatrix);
                                    glm::mat4 model = glm::translate(glm::mat4(1.0f), chunk.position);
                                    shader->SetMat4("u_MTransform", trans.GetTransform() * model);

                                    const VertexArray& vertexArray = chunk.component->GetVertexArrayRef();
                                    vertexArray.Bind();

                                    glDrawArrays(GL_TRIANGLES, 0, chunk.component->GetVertexCount());

                                    vertexArray.UnBind();
                                }
                            }
                        }

                        shader->UnBind();
                    }

                    shadowMapBuffer.UnBind();
                }
                glCullFace(GL_BACK);
            }

            isShadowMapGenerated = true;
        }

        // VoxelGI_VoxelTexture
        static bool isVoxelGI_VoxelTextureInitialized = false;
        if (isVoxelGI_VoxelTextureInitialized == false)
        {
            glViewport(0, 0, m_VoxelGI_settings.voxel_grid_resolution, m_VoxelGI_settings.voxel_grid_resolution);
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);

            glBindTexture(GL_TEXTURE_3D, (GLuint)(size_t)m_VoxelGI_VoxelTexture->GetTextureID());
            glClearTexImage((GLuint)(size_t)m_VoxelGI_VoxelTexture->GetTextureID(),
                            0,
                            GL_RGBA,
                            GL_FLOAT,
                            glm::value_ptr(glm::vec4(0.0f)));
            glBindTexture(GL_TEXTURE_3D, 0);

            for (auto [entity, name, trans, model] : model_view.each())
            {
                AStaticMesh* staticMesh_actor = static_cast<AStaticMesh*>(model.GetOwner());
                MMaterial*   material         = static_cast<MMaterial*>(staticMesh_actor->GetMaterial());
                std::string  materialType     = material->GetMaterialType();

                const auto meshes    = model.GetStaticMesh().m_Meshes;
                const auto materials = model.GetStaticMesh().m_Materials;
                for (int i = 0; i < meshes.size(); ++i)
                {
                    const auto mesh              = meshes[i];
                    const auto material_basicPbr = static_cast<MBasicPbr*>(materials[i]);

                    auto shader = m_ShaderLibrary.Get("VoxelGI_Voxelization");
                    shader->Bind();

                    shader->SetMat4("M", trans.GetTransform());
                    shader->SetFloat3("u_scene_voxel_scale", m_VoxelGI_settings.scene_voxel_scale);
                    shader->SetInt("u_tex_voxelgrid", 0);
                    glActiveTexture(GL_TEXTURE0);
                    m_VoxelGI_VoxelTexture->Bind(0);
                    glBindImageTexture(0,
                                       (GLuint)(size_t)m_VoxelGI_VoxelTexture->GetTextureID(),
                                       0,
                                       GL_TRUE,
                                       0,
                                       GL_WRITE_ONLY,
                                       GL_RGBA16F);

                    glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                    shader->SetFloat3("camPos", camPos);

                    auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

                    shader->SetInt("irradianceMap", 15);
                    sky_cubeMap->BindIrradianceTexture(15);
                    const auto& sphericlaHarmonicsParameters = sky_cubeMap->GetSphereHarmonicsParametersRef();
                    for (int i = 0; i < 9; ++i)
                    {
                        shader->SetFloat3("sphericlaHarmonicsParameters[" + std::to_string(i) + "]",
                                          sphereHarmonicsParameters_a[i]);
                    }

                    shader->SetInt("prefilterMap", 16);
                    sky_cubeMap->BindPrefilterTexture(16);
                    shader->SetInt("brdfLUT", 17);
                    sky_cubeMap->BindBrdfLutTexture(17);

                    if (materialType == "BasicPbr")
                    {
                        material_basicPbr->BindAllMap(shader);
                    }

                    int ligth_num = 0;
                    for (auto [lightEntity, lightName, lightTrans, light] : pointlight_view.each())
                    {
                        shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                          lightTrans.GetPosition());
                        shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]", light.GetColorRef());
                        shader->SetFloat("pointLightIntensities[" + std::to_string(ligth_num) + "]",
                                         light.GetIntensityRef());

                        shader->SetInt("pointShadowCubeMaps[" + std::to_string(ligth_num) + "]", 50 + ligth_num);
                        light.GetShadowCubeMapBufferRef().BindTexture(50 + ligth_num);
                        shader->SetFloat("pointShadowCubeMapsNearPlane[" + std::to_string(ligth_num) + "]", 1.0f);
                        shader->SetFloat("pointShadowCubeMapsFarPlane[" + std::to_string(ligth_num) + "]", 10.0f);

                        ligth_num++;
                    }
                    shader->SetInt("numPointLights", ligth_num);

                    int dirctionallight_num = 0;
                    for (auto [lightEntity, lightName, lightTrans, light] : dirctionallight_view.each())
                    {
                        const glm::mat4 lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.5f, 120.0f);
                        ;
                        glm::vec3 cameraPosition       = actor_mainCamera->GetTransformComponent().GetPosition();
                        cameraPosition.y               = 0.0f;
                        const glm::vec3 lightPosition  = lightTrans.GetPosition();
                        const glm::vec3 lightDirection = light.GetDirectionRef();
                        const glm::mat4 lightView =
                            glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
                        const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

                        shader->SetFloat3("directionalLightDirections[" + std::to_string(dirctionallight_num) + "]",
                                          light.GetDirectionRef());
                        shader->SetFloat3("directionalLightColors[" + std::to_string(dirctionallight_num) + "]",
                                          light.GetColorRef());
                        shader->SetFloat("directionalLightIntensities[" + std::to_string(dirctionallight_num) + "]",
                                         light.GetIntensityRef());
                        shader->SetMat4("directionalLightMatrices[" + std::to_string(dirctionallight_num) + "]",
                                        lightSpaceMatrix);
                        shader->SetInt("directionalShadowMaps[" + std::to_string(dirctionallight_num) + "]",
                                       150 + dirctionallight_num);

                        light.GetShadowMapBufferRef().BindTexture(150 + dirctionallight_num);

                        dirctionallight_num++;
                    }
                    shader->SetInt("numDirectionalLights", dirctionallight_num);
                    shader->SetFloat("PCSS_FilterRadius", m_PCSS_FilterRadius);

                    mesh->m_VertexArray->Bind();
                    RenderCommand::DrawIndexed(mesh->m_VertexArray);
                    mesh->m_VertexArray->UnBind();

                    shader->UnBind();
                }
            }

            // generate mipmap
            glBindTexture(GL_TEXTURE_3D, (GLuint)(size_t)m_VoxelGI_VoxelTexture->GetTextureID());
            glGenerateMipmap(GL_TEXTURE_3D);
            glBindTexture(GL_TEXTURE_3D, 0);

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            isVoxelGI_VoxelTextureInitialized = true;
        }

        // Render to GeometryBuffer
        glEnable(GL_CULL_FACE);

        m_GeometryBuffer->Bind();
        RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

        RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        RenderCommand::Clear();

        // use a range-for
        for (auto [entity, name, trans, model] : model_view.each())
        {
            AStaticMesh* staticMesh_actor = static_cast<AStaticMesh*>(model.GetOwner());
            MMaterial*   material         = static_cast<MMaterial*>(staticMesh_actor->GetMaterial());
            std::string  materialType     = material->GetMaterialType();

            if (staticMesh_actor->GetVisible() == false)
            {
                continue;
            }

            if (materialType == "BasicPbr")
            {
                auto shader = m_ShaderLibrary.Get("GBuffer");
                shader->Bind();

                shader->SetMat4("u_MProjection", m_PMatrix);
                shader->SetMat4("u_MView", m_VMatrix);
                shader->SetMat4("u_MTransform", trans.GetTransform());

                const Frustum frustum =
                    m_MainCamera->CreateFrustumFromCamera(actor_mainCamera->GetTransformComponent());
                const auto& materials = model.GetStaticMesh().m_LoadedMaterials;
                const auto& meshes    = model.GetStaticMesh().m_LoadedMeshes;

                for (const auto& meshArray : meshes)
                {
                    std::string materialName      = meshArray.first;
                    const auto  material_basicPbr = static_cast<MBasicPbr*>(materials.at(materialName));
                    material_basicPbr->BindAllMap(shader);

                    uint32_t drawedMeshCount = 0;
                    for (const auto& mesh : meshArray.second)
                    {
                        const FrustumVolume* frustumVolume = mesh->GetFrustumVolume();

                        if (frustumVolume->isOnFrustum(frustum, trans) == false)
                        {
                            continue;
                        }
                        // material_basicPbr->BufferTextures();

                        Renderer::Submit(mesh->m_VertexArray, shader, m_VPMatrix, trans.GetTransform());
                        drawedMeshCount++;
                    }
                    material_basicPbr->UnBindAllMap(shader);

                    if (drawedMeshCount == 0)
                    {
                        // material_basicPbr->ClearTextures();
                    }
                }
                shader->UnBind();
            }
        }

        // draw chunks
        for (auto [entity, name, trans, world] : chunkarrayworld_view.each())
        {
            glm::vec3 cameraPosition = actor_mainCamera->GetTransformComponent().GetPosition();
            auto      shader         = m_ShaderLibrary.Get("ChunkGBuffer");
            shader->Bind();

            for (int x = 0; x < world.GetWorldSizeX(); ++x)
            {
                for (int y = 0; y < world.GetWorldSizeY(); ++y)
                {
                    for (int z = 0; z < world.GetWorldSizeZ(); ++z)
                    {
                        const UChunkArrayWorldComponent::Chunk& chunk = world.GetChunkRef(x, y, z);

                        glm::vec3 chunkPosition = chunk.position + glm::vec3(16.0f, 16.0f, 16.0f);

                        if (glm::length(cameraPosition - chunkPosition) > 128.0f || chunk.component == nullptr)
                        {
                            continue;
                        }

                        shader->SetMat4("u_MProjection", m_PMatrix);
                        shader->SetMat4("u_MView", m_VMatrix);

                        glm::mat4 model = glm::translate(glm::mat4(1.0f), chunk.position);
                        shader->SetMat4("u_MTransform", trans.GetTransform() * model);

                        const Texture2D& texture = chunk.component->GetTextureRef();

                        shader->SetBool("useAlbedoMap", true);
                        shader->SetInt("albedoMap", 0);
                        texture.Bind(0);

                        const VertexArray& vertexArray = chunk.component->GetVertexArrayRef();
                        vertexArray.Bind();

                        glDrawArrays(GL_TRIANGLES, 0, chunk.component->GetVertexCount());
                        texture.UnBind(0);

                        vertexArray.UnBind();
                    }
                }
            }

            shader->UnBind();
        }

        // draw skinned mesh
        for (auto [entity, name, trans, skinnedmesh] : skinnedmesh_view.each())
        {
            APawn&      skinnedmesh_actor = *static_cast<APawn*>(skinnedmesh.GetOwner());
            MMaterial*  material          = static_cast<MMaterial*>(skinnedmesh.GetMaterial());
            std::string materialType      = material->GetMaterialType();

            if (skinnedmesh_actor.GetVisible() == false)
            {
                continue;
            }

            if (materialType == "BasicPbr")
            {
                auto skinnedMeshShader = m_ShaderLibrary.Get("Skinned");
                skinnedMeshShader->Bind();

                MBasicPbr* material_basicPbr = static_cast<MBasicPbr*>(material);
                material_basicPbr->BindAllMap(skinnedMeshShader);

                glm::mat4 transform = skinnedmesh.GetTransformComponentRef().GetTransform();
                skinnedmesh.DrawSkinnedMesh(skinnedMeshShader, transform, m_PMatrix, m_VMatrix);

                material_basicPbr->UnBindAllMap(skinnedMeshShader);

                skinnedMeshShader->UnBind();
            }
        }

        // draw MotionMatching
        for (auto [entity, name, trans, motionmatching] : motionmatching_view.each())
        {
            USkinnedMeshComponent& skinnedmesh = motionmatching.GetSkinnedMeshComponent0Ref();

            APawn&      skinnedmesh_actor = *static_cast<APawn*>(motionmatching.GetOwner());
            MMaterial*  material          = static_cast<MMaterial*>(skinnedmesh.GetMaterial());
            std::string materialType      = material->GetMaterialType();

            if (skinnedmesh_actor.GetVisible() == false)
            {
                continue;
            }

            if (materialType == "BasicPbr")
            {
                auto skinnedMeshShader = m_ShaderLibrary.Get("Skinned");
                skinnedMeshShader->Bind();

                MBasicPbr* material_basicPbr = static_cast<MBasicPbr*>(material);
                material_basicPbr->BindAllMap(skinnedMeshShader);

                glm::mat4 transform = skinnedmesh.GetTransformComponentRef().GetTransform();
                skinnedmesh.DrawSkinnedMesh(skinnedMeshShader, transform, m_PMatrix, m_VMatrix);

                material_basicPbr->UnBindAllMap(skinnedMeshShader);

                skinnedMeshShader->UnBind();
            }
        }

        m_GeometryBuffer->UnBind();
        glDisable(GL_CULL_FACE);

        // Render to SSAOBuffer
        m_MainCamera->GetCamera().SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_MainCamera->GetCamera().RecalculateProjectionMatrix();

        m_VMatrix =
            glm::inverse(actor_mainCamera->GetTransformComponent().GetTransform() * m_MainCamera->GetTransform());
        m_PMatrix  = m_MainCamera->GetCamera().GetProjectionMatrix();
        m_VPMatrix = m_PMatrix * m_VMatrix;

        m_SSAOBuffer->Bind();
        RenderCommand::SetViewPort(0, 0, m_GeometryBuffer->GetWidth(), m_GeometryBuffer->GetHeight());

        RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        RenderCommand::Clear();

        auto computeAO_shader = m_ShaderLibrary.Get("ComputeAO");
        computeAO_shader->Bind();

        computeAO_shader->SetInt("g_ViewPosition", 0);
        m_GeometryBuffer->BindViewPositionTexture(0);
        computeAO_shader->SetInt("g_ViewNormal", 1);
        m_GeometryBuffer->BindViewNormalTexture(1);
        computeAO_shader->SetInt("g_Depth", 2);
        m_GeometryBuffer->BindDepthTexture(2);

        computeAO_shader->SetInt("texNoise", 3);
        m_SSAOBuffer->BindNoiseTexture(3);

        std::vector<glm::vec3>& kernel = m_SSAOBuffer->GetSSAOKernel();
        for (unsigned int i = 0; i < kernel.size(); ++i)
        {
            computeAO_shader->SetFloat3("samples[" + std::to_string(i) + "]", kernel[i]);
        }
        computeAO_shader->SetMat4("projection", m_PMatrix);

        computeAO_shader->SetFloat("radius", m_SSAOBuffer->GetRadiusRef());
        computeAO_shader->SetFloat("bias", m_SSAOBuffer->GetBiasRef());
        computeAO_shader->SetFloat("power", m_SSAOBuffer->GetPowerRef());

        computeAO_shader->SetFloat("screenWidth", m_FrameRenderBuffer->GetWidth());
        computeAO_shader->SetFloat("screenHeight", m_FrameRenderBuffer->GetHeight());

        computeAO_shader->Bind();
        RenderCommand::RenderToQuad();

        m_SSAOBuffer->UnBindTexture(3);

        m_GeometryBuffer->UnBindTexture(2);
        m_GeometryBuffer->UnBindTexture(1);
        m_GeometryBuffer->UnBindTexture(0);

        computeAO_shader->UnBind();

        m_SSAOBuffer->UnBind();

        // Render to Skybox
        m_MainCamera->GetCamera().SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_MainCamera->GetCamera().RecalculateProjectionMatrix();

        m_VMatrix =
            glm::inverse(actor_mainCamera->GetTransformComponent().GetTransform() * m_MainCamera->GetTransform());
        m_PMatrix  = m_MainCamera->GetCamera().GetProjectionMatrix();
        m_VPMatrix = m_PMatrix * m_VMatrix;

        // skybox shading
        {
            m_FrameRenderBuffer_skybox->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            if (m_RenderSkybox)
            {
                // draw skybox
                // use a range-for
                for (auto [entity, name, trans, skybox] : skybox_view.each())
                {
                    auto shader = m_ShaderLibrary.Get("Skybox");
                    Renderer::SetShaderUniform(shader, "mipLevel", m_VisPrePrefilterMipLevel);

                    auto vpmat = m_PMatrix * glm::mat4(glm::mat3(m_VMatrix));
                    skybox.Draw(shader, vpmat);
                }
            }

            m_FrameRenderBuffer_skybox->UnBind();
        }

        // DirectLighting_diffuse Shading
        {
            m_FrameRenderBuffer_DirectLighting_diffuse->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto deferred_shader = m_ShaderLibrary.Get("DirectLighting_diffuse");
            deferred_shader->Bind();

            deferred_shader->SetInt("g_ViewPosition", 0);
            m_GeometryBuffer->BindViewPositionTexture(0);
            deferred_shader->SetInt("g_ViewNormal", 1);
            m_GeometryBuffer->BindViewNormalTexture(1);
            deferred_shader->SetInt("g_Diffuse", 2);
            m_GeometryBuffer->BindDiffuseTexture(2);
            deferred_shader->SetInt("g_Depth", 3);
            m_GeometryBuffer->BindDepthTexture(3);
            deferred_shader->SetInt("g_Roughness", 4);
            m_GeometryBuffer->BindRoughnessTexture(4);
            deferred_shader->SetInt("g_Specular", 5);
            m_GeometryBuffer->BindSpecularTexture(5);
            deferred_shader->SetInt("g_WorldPosition", 6);
            m_GeometryBuffer->BindWorldPositionTexture(6);
            deferred_shader->SetInt("g_WorldNormal", 7);
            m_GeometryBuffer->BindWorldNormalTexture(7);
            deferred_shader->SetInt("g_AO", 8);
            m_SSAOBuffer->BindSSAOTexture(8);

            int ligth_num = 0;
            for (auto [entity, name, trans, light] : pointlight_view.each())
            {
                deferred_shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                           trans.GetPosition());
                deferred_shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]", light.GetColorRef());
                deferred_shader->SetFloat("pointLightIntensities[" + std::to_string(ligth_num) + "]",
                                          light.GetIntensityRef());

                deferred_shader->SetInt("pointShadowCubeMaps[" + std::to_string(ligth_num) + "]", 50 + ligth_num);
                light.GetShadowCubeMapBufferRef().BindTexture(50 + ligth_num);
                deferred_shader->SetFloat("pointShadowCubeMapsNearPlane[" + std::to_string(ligth_num) + "]", 1.0f);
                deferred_shader->SetFloat("pointShadowCubeMapsFarPlane[" + std::to_string(ligth_num) + "]", 10.0f);

                ligth_num++;
            }
            deferred_shader->SetInt("numPointLights", ligth_num);

            int dirctionallight_num = 0;
            for (auto [entity, name, trans, light] : dirctionallight_view.each())
            {
                const glm::mat4 lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.5f, 120.0f);
                ;
                glm::vec3 cameraPosition       = actor_mainCamera->GetTransformComponent().GetPosition();
                cameraPosition.y               = 0.0f;
                const glm::vec3 lightPosition  = trans.GetPosition();
                const glm::vec3 lightDirection = light.GetDirectionRef();
                const glm::mat4 lightView =
                    glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
                const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

                deferred_shader->SetFloat3("directionalLightDirections[" + std::to_string(dirctionallight_num) + "]",
                                           light.GetDirectionRef());
                deferred_shader->SetFloat3("directionalLightColors[" + std::to_string(dirctionallight_num) + "]",
                                           light.GetColorRef());
                deferred_shader->SetFloat("directionalLightIntensities[" + std::to_string(dirctionallight_num) + "]",
                                          light.GetIntensityRef());
                deferred_shader->SetMat4("directionalLightMatrices[" + std::to_string(dirctionallight_num) + "]",
                                         lightSpaceMatrix);
                deferred_shader->SetInt("directionalShadowMaps[" + std::to_string(dirctionallight_num) + "]",
                                        150 + dirctionallight_num);

                light.GetShadowMapBufferRef().BindTexture(150 + dirctionallight_num);

                dirctionallight_num++;
            }
            deferred_shader->SetInt("numDirectionalLights", dirctionallight_num);
            deferred_shader->SetFloat("PCSS_FilterRadius", m_PCSS_FilterRadius);

            glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            deferred_shader->SetFloat3("camPos", camPos);

            auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

            deferred_shader->SetInt("irradianceMap", 15);
            sky_cubeMap->BindIrradianceTexture(15);
            deferred_shader->SetInt("prefilterMap", 16);
            sky_cubeMap->BindPrefilterTexture(16);
            deferred_shader->SetInt("brdfLUT", 17);
            sky_cubeMap->BindBrdfLutTexture(17);

            RenderCommand::RenderToQuad();

            sky_cubeMap->UnBind(17);
            sky_cubeMap->UnBind(16);
            sky_cubeMap->UnBind(15);

            m_SSAOBuffer->UnBindTexture(8);
            m_GeometryBuffer->UnBindTexture(7);
            m_GeometryBuffer->UnBindTexture(6);
            m_GeometryBuffer->UnBindTexture(5);
            m_GeometryBuffer->UnBindTexture(4);
            m_GeometryBuffer->UnBindTexture(3);
            m_GeometryBuffer->UnBindTexture(2);
            m_GeometryBuffer->UnBindTexture(1);
            m_GeometryBuffer->UnBindTexture(0);

            deferred_shader->UnBind();
            m_FrameRenderBuffer_DirectLighting_diffuse->UnBind();
        }
        // DirectLighting_specular Shading
        {
            m_FrameRenderBuffer_DirectLighting_specular->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto deferred_shader = m_ShaderLibrary.Get("DirectLighting_specular");
            deferred_shader->Bind();

            deferred_shader->SetInt("g_ViewPosition", 0);
            m_GeometryBuffer->BindViewPositionTexture(0);
            deferred_shader->SetInt("g_ViewNormal", 1);
            m_GeometryBuffer->BindViewNormalTexture(1);
            deferred_shader->SetInt("g_Diffuse", 2);
            m_GeometryBuffer->BindDiffuseTexture(2);
            deferred_shader->SetInt("g_Depth", 3);
            m_GeometryBuffer->BindDepthTexture(3);
            deferred_shader->SetInt("g_Roughness", 4);
            m_GeometryBuffer->BindRoughnessTexture(4);
            deferred_shader->SetInt("g_Specular", 5);
            m_GeometryBuffer->BindSpecularTexture(5);
            deferred_shader->SetInt("g_WorldPosition", 6);
            m_GeometryBuffer->BindWorldPositionTexture(6);
            deferred_shader->SetInt("g_WorldNormal", 7);
            m_GeometryBuffer->BindWorldNormalTexture(7);
            deferred_shader->SetInt("g_AO", 8);
            m_SSAOBuffer->BindSSAOTexture(8);

            int ligth_num = 0;
            for (auto [entity, name, trans, light] : pointlight_view.each())
            {
                deferred_shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                           trans.GetPosition());
                deferred_shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]", light.GetColorRef());
                deferred_shader->SetFloat("pointLightIntensities[" + std::to_string(ligth_num) + "]",
                                          light.GetIntensityRef());

                deferred_shader->SetInt("pointShadowCubeMaps[" + std::to_string(ligth_num) + "]", 50 + ligth_num);
                light.GetShadowCubeMapBufferRef().BindTexture(50 + ligth_num);
                deferred_shader->SetFloat("pointShadowCubeMapsNearPlane[" + std::to_string(ligth_num) + "]", 1.0f);
                deferred_shader->SetFloat("pointShadowCubeMapsFarPlane[" + std::to_string(ligth_num) + "]", 10.0f);

                ligth_num++;
            }
            deferred_shader->SetInt("numPointLights", ligth_num);

            int dirctionallight_num = 0;
            for (auto [entity, name, trans, light] : dirctionallight_view.each())
            {
                const glm::mat4 lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.5f, 120.0f);
                const glm::vec3 lightPosition   = trans.GetPosition();
                const glm::vec3 lightDirection  = light.GetDirectionRef();
                const glm::mat4 lightView =
                    glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
                const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

                deferred_shader->SetFloat3("directionalLightDirections[" + std::to_string(dirctionallight_num) + "]",
                                           light.GetDirectionRef());
                deferred_shader->SetFloat3("directionalLightColors[" + std::to_string(dirctionallight_num) + "]",
                                           light.GetColorRef());
                deferred_shader->SetFloat("directionalLightIntensities[" + std::to_string(dirctionallight_num) + "]",
                                          light.GetIntensityRef());
                deferred_shader->SetMat4("directionalLightMatrices[" + std::to_string(dirctionallight_num) + "]",
                                         lightSpaceMatrix);
                deferred_shader->SetInt("directionalShadowMaps[" + std::to_string(dirctionallight_num) + "]",
                                        150 + dirctionallight_num);

                light.GetShadowMapBufferRef().BindTexture(150 + dirctionallight_num);

                dirctionallight_num++;
            }
            deferred_shader->SetInt("numDirectionalLights", dirctionallight_num);
            deferred_shader->SetFloat("PCSS_FilterRadius", m_PCSS_FilterRadius);

            glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            deferred_shader->SetFloat3("camPos", camPos);

            auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

            deferred_shader->SetInt("irradianceMap", 15);
            sky_cubeMap->BindIrradianceTexture(15);
            deferred_shader->SetInt("prefilterMap", 16);
            sky_cubeMap->BindPrefilterTexture(16);
            deferred_shader->SetInt("brdfLUT", 17);
            sky_cubeMap->BindBrdfLutTexture(17);

            RenderCommand::RenderToQuad();

            sky_cubeMap->UnBind(17);
            sky_cubeMap->UnBind(16);
            sky_cubeMap->UnBind(15);

            m_SSAOBuffer->UnBindTexture(8);
            m_GeometryBuffer->UnBindTexture(7);
            m_GeometryBuffer->UnBindTexture(6);
            m_GeometryBuffer->UnBindTexture(5);
            m_GeometryBuffer->UnBindTexture(4);
            m_GeometryBuffer->UnBindTexture(3);
            m_GeometryBuffer->UnBindTexture(2);
            m_GeometryBuffer->UnBindTexture(1);
            m_GeometryBuffer->UnBindTexture(0);

            deferred_shader->UnBind();
            m_FrameRenderBuffer_DirectLighting_specular->UnBind();
        }
        // EnvironmentLighting_diffuse Shading
        {
            m_FrameRenderBuffer_EnvironmentLighting_diffuse->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto deferred_shader = m_ShaderLibrary.Get("EnvironmentLighting_diffuse");
            deferred_shader->Bind();

            deferred_shader->SetInt("g_ViewPosition", 0);
            m_GeometryBuffer->BindViewPositionTexture(0);
            deferred_shader->SetInt("g_ViewNormal", 1);
            m_GeometryBuffer->BindViewNormalTexture(1);
            deferred_shader->SetInt("g_Diffuse", 2);
            m_GeometryBuffer->BindDiffuseTexture(2);
            deferred_shader->SetInt("g_Depth", 3);
            m_GeometryBuffer->BindDepthTexture(3);
            deferred_shader->SetInt("g_Roughness", 4);
            m_GeometryBuffer->BindRoughnessTexture(4);
            deferred_shader->SetInt("g_Specular", 5);
            m_GeometryBuffer->BindSpecularTexture(5);
            deferred_shader->SetInt("g_WorldPosition", 6);
            m_GeometryBuffer->BindWorldPositionTexture(6);
            deferred_shader->SetInt("g_WorldNormal", 7);
            m_GeometryBuffer->BindWorldNormalTexture(7);
            deferred_shader->SetInt("g_AO", 8);
            m_SSAOBuffer->BindSSAOTexture(8);

            int ligth_num = 0;
            for (auto [entity, name, trans, light] : pointlight_view.each())
            {
                deferred_shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                           trans.GetPosition());
                deferred_shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]", light.GetColorRef());

                ligth_num++;
            }
            deferred_shader->SetInt("numPointLights", ligth_num);

            glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            deferred_shader->SetFloat3("camPos", camPos);

            auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

            // deferred_shader->SetInt("irradianceMap", 15);
            // sky_cubeMap->BindIrradianceTexture(15);
            const auto& sphericlaHarmonicsParameters = sky_cubeMap->GetSphereHarmonicsParametersRef();
            for (int i = 0; i < 9; ++i)
            {
                deferred_shader->SetFloat3("sphericlaHarmonicsParameters[" + std::to_string(i) + "]",
                                           sphereHarmonicsParameters_a[i]);
            }

            deferred_shader->SetInt("prefilterMap", 16);
            sky_cubeMap->BindPrefilterTexture(16);
            deferred_shader->SetInt("brdfLUT", 17);
            sky_cubeMap->BindBrdfLutTexture(17);

            RenderCommand::RenderToQuad();

            sky_cubeMap->UnBind(17);
            sky_cubeMap->UnBind(16);
            sky_cubeMap->UnBind(15);

            m_SSAOBuffer->UnBindTexture(8);
            m_GeometryBuffer->UnBindTexture(7);
            m_GeometryBuffer->UnBindTexture(6);
            m_GeometryBuffer->UnBindTexture(5);
            m_GeometryBuffer->UnBindTexture(4);
            m_GeometryBuffer->UnBindTexture(3);
            m_GeometryBuffer->UnBindTexture(2);
            m_GeometryBuffer->UnBindTexture(1);
            m_GeometryBuffer->UnBindTexture(0);

            deferred_shader->UnBind();
            m_FrameRenderBuffer_EnvironmentLighting_diffuse->UnBind();
        }
        // EnvironmentLighting_specular Shading
        {
            m_FrameRenderBuffer_EnvironmentLighting_specular->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto deferred_shader = m_ShaderLibrary.Get("EnvironmentLighting_specular");
            deferred_shader->Bind();

            deferred_shader->SetInt("g_ViewPosition", 0);
            m_GeometryBuffer->BindViewPositionTexture(0);
            deferred_shader->SetInt("g_ViewNormal", 1);
            m_GeometryBuffer->BindViewNormalTexture(1);
            deferred_shader->SetInt("g_Diffuse", 2);
            m_GeometryBuffer->BindDiffuseTexture(2);
            deferred_shader->SetInt("g_Depth", 3);
            m_GeometryBuffer->BindDepthTexture(3);
            deferred_shader->SetInt("g_Roughness", 4);
            m_GeometryBuffer->BindRoughnessTexture(4);
            deferred_shader->SetInt("g_Specular", 5);
            m_GeometryBuffer->BindSpecularTexture(5);
            deferred_shader->SetInt("g_WorldPosition", 6);
            m_GeometryBuffer->BindWorldPositionTexture(6);
            deferred_shader->SetInt("g_WorldNormal", 7);
            m_GeometryBuffer->BindWorldNormalTexture(7);
            deferred_shader->SetInt("g_AO", 8);
            m_SSAOBuffer->BindSSAOTexture(8);

            int ligth_num = 0;
            for (auto [entity, name, trans, light] : pointlight_view.each())
            {
                deferred_shader->SetFloat3("pointLightPositions[" + std::to_string(ligth_num) + "]",
                                           trans.GetPosition());
                deferred_shader->SetFloat3("pointLightColors[" + std::to_string(ligth_num) + "]", light.GetColorRef());

                ligth_num++;
            }
            deferred_shader->SetInt("numPointLights", ligth_num);

            glm::vec3 camPos = glm::vec3(glm::inverse(m_VMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            deferred_shader->SetFloat3("camPos", camPos);

            auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

            deferred_shader->SetInt("irradianceMap", 15);
            sky_cubeMap->BindIrradianceTexture(15);
            deferred_shader->SetInt("prefilterMap", 16);
            m_Probe_CubeMap->BindPrefilterTexture(16);
            deferred_shader->SetInt("brdfLUT", 17);
            sky_cubeMap->BindBrdfLutTexture(17);

            RenderCommand::RenderToQuad();

            sky_cubeMap->UnBind(17);
            sky_cubeMap->UnBind(16);
            sky_cubeMap->UnBind(15);

            m_SSAOBuffer->UnBindTexture(8);
            m_GeometryBuffer->UnBindTexture(7);
            m_GeometryBuffer->UnBindTexture(6);
            m_GeometryBuffer->UnBindTexture(5);
            m_GeometryBuffer->UnBindTexture(4);
            m_GeometryBuffer->UnBindTexture(3);
            m_GeometryBuffer->UnBindTexture(2);
            m_GeometryBuffer->UnBindTexture(1);
            m_GeometryBuffer->UnBindTexture(0);

            deferred_shader->UnBind();
            m_FrameRenderBuffer_EnvironmentLighting_specular->UnBind();
        }
        // VoxelGI
        {
            m_FrameRenderBuffer_VoxelGI->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto voxelGI_shader = m_ShaderLibrary.Get("VoxelGI_Conetracing");
            voxelGI_shader->Bind();

            voxelGI_shader->SetInt("u_tex_voxelgrid", 0);
            m_VoxelGI_VoxelTexture->Bind(0);
            voxelGI_shader->SetInt("g_WorldPosition", 1);
            m_GeometryBuffer->BindWorldPositionTexture(1);
            voxelGI_shader->SetInt("g_WorldNormal", 2);
            m_GeometryBuffer->BindWorldNormalTexture(2);
            voxelGI_shader->SetInt("g_Diffuse", 3);
            m_GeometryBuffer->BindDiffuseTexture(3);
            voxelGI_shader->SetInt("g_Specular", 4);
            m_GeometryBuffer->BindSpecularTexture(4);

            glm::vec3 cameraPosition = actor_mainCamera->GetTransformComponent().GetPosition();
            voxelGI_shader->SetFloat3("u_camera_world_position", cameraPosition);
            voxelGI_shader->SetFloat3("u_scene_voxel_scale", m_VoxelGI_settings.scene_voxel_scale);

            voxelGI_shader->SetFloat("settings.diffuse.aperture", m_VoxelGI_settings.diffuse.aperture);
            voxelGI_shader->SetFloat("settings.diffuse.sampling_factor", m_VoxelGI_settings.diffuse.sampling_factor);
            voxelGI_shader->SetFloat("settings.diffuse.distance_offset", m_VoxelGI_settings.diffuse.distance_offset);
            voxelGI_shader->SetFloat("settings.diffuse.max_distance", m_VoxelGI_settings.diffuse.max_distance);
            voxelGI_shader->SetFloat("settings.diffuse.result_intensity", m_VoxelGI_settings.diffuse.result_intensity);

            voxelGI_shader->SetFloat("settings.specular.aperture", m_VoxelGI_settings.specular.aperture);
            voxelGI_shader->SetFloat("settings.specular.sampling_factor", m_VoxelGI_settings.specular.sampling_factor);
            voxelGI_shader->SetFloat("settings.specular.distance_offset", m_VoxelGI_settings.specular.distance_offset);
            voxelGI_shader->SetFloat("settings.specular.max_distance", m_VoxelGI_settings.specular.max_distance);
            voxelGI_shader->SetFloat("settings.specular.result_intensity",
                                     m_VoxelGI_settings.specular.result_intensity);

            voxelGI_shader->SetInt("settings.voxel_grid_resolution", m_VoxelGI_settings.voxel_grid_resolution);
            voxelGI_shader->SetInt("settings.max_mipmap_level", m_VoxelGI_settings.max_mipmap_level);

            RenderCommand::RenderToQuad();

            m_GeometryBuffer->UnBindTexture(4);
            m_GeometryBuffer->UnBindTexture(3);
            m_GeometryBuffer->UnBindTexture(2);
            m_GeometryBuffer->UnBindTexture(1);
            m_VoxelGI_VoxelTexture->UnBind(0);

            voxelGI_shader->UnBind();
            m_FrameRenderBuffer_VoxelGI->UnBind();
        }

        // ssr
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_ssr->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto ssr_shader = m_ShaderLibrary.Get("ScreenSpaceReflection");
            ssr_shader->Bind();

            ssr_shader->SetInt("g_DirectLightinging_diffuse", 0);
            m_FrameRenderBuffer_DirectLighting_diffuse->BindTexture(0);
            ssr_shader->SetInt("g_DirectLightinging_specular", 1);
            m_FrameRenderBuffer_DirectLighting_specular->BindTexture(1);
            ssr_shader->SetInt("g_EnvironmentLighting_diffuse", 2);
            m_FrameRenderBuffer_EnvironmentLighting_diffuse->BindTexture(2);
            ssr_shader->SetInt("g_EnvironmentLighting_specular", 3);
            m_FrameRenderBuffer_EnvironmentLighting_specular->BindTexture(3);
            ssr_shader->SetInt("g_ViewPosition", 4);
            m_GeometryBuffer->BindViewPositionTexture(4);
            ssr_shader->SetInt("g_ViewNormal", 5);
            m_GeometryBuffer->BindViewNormalTexture(5);
            ssr_shader->SetInt("g_Diffuse", 6);
            m_GeometryBuffer->BindDiffuseTexture(6);
            ssr_shader->SetInt("g_Roughness", 7);
            m_GeometryBuffer->BindRoughnessTexture(7);
            ssr_shader->SetInt("g_Specular", 8);
            m_GeometryBuffer->BindSpecularTexture(8);
            ssr_shader->SetInt("g_WorldPosition", 9);
            m_GeometryBuffer->BindWorldPositionTexture(9);

            ssr_shader->SetFloat("rayStep", m_SSR_settings.rayStep);
            ssr_shader->SetFloat("minRayStep", m_SSR_settings.minRayStep);
            ssr_shader->SetFloat("maxSteps", m_SSR_settings.maxSteps);
            ssr_shader->SetInt("numBinarySearchSteps", m_SSR_settings.numBinarySearchSteps);
            ssr_shader->SetFloat("reflectionSpecularFalloffExponent", m_SSR_settings.reflectionSpecularFalloffExponent);
            ssr_shader->SetBool("debug", m_SSR_settings.debug);
            ssr_shader->SetFloat("refBias", m_SSR_settings.refBias);

            ssr_shader->SetMat4("u_MProjection", m_PMatrix);
            ssr_shader->SetMat4("u_MView", m_VMatrix);
            ssr_shader->SetMat4("u_MInvProjection", glm::inverse(m_PMatrix));
            ssr_shader->SetMat4("u_MInvView", glm::inverse(m_VMatrix));

            RenderCommand::RenderToQuad();

            m_GeometryBuffer->UnBindTexture(9);
            m_GeometryBuffer->UnBindTexture(8);
            m_GeometryBuffer->UnBindTexture(7);
            m_GeometryBuffer->UnBindTexture(6);
            m_GeometryBuffer->UnBindTexture(5);
            m_GeometryBuffer->UnBindTexture(4);
            m_FrameRenderBuffer_EnvironmentLighting_specular->UnBindTexture(3);
            m_FrameRenderBuffer_EnvironmentLighting_diffuse->UnBindTexture(2);
            m_FrameRenderBuffer_DirectLighting_specular->UnBindTexture(1);
            m_FrameRenderBuffer_DirectLighting_diffuse->UnBindTexture(0);

            ssr_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_ssr);
            glEnable(GL_DEPTH_TEST);
        }
        // blur ssr
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_ssr_blur->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto blur_shader = m_ShaderLibrary.Get("GaussianBlur");
            blur_shader->Bind();

            blur_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_ssr->BindTexture(0);
            blur_shader->SetFloat2("screenSize",
                                   glm::vec2(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight()) / 3.0f);

            RenderCommand::RenderToQuad();

            m_FrameRenderBuffer_ssr->UnBindTexture(0);

            blur_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_ssr_blur);
            glEnable(GL_DEPTH_TEST);
        }

        // composite
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto composite_shader = m_ShaderLibrary.Get("Composite");
            composite_shader->Bind();

            composite_shader->SetFloat("k_dir_diffuse", m_Dir_Diffuse);
            composite_shader->SetFloat("k_dir_specular", m_Dir_Specular);
            composite_shader->SetFloat("k_env_diffuse", m_Env_Diffuse);
            composite_shader->SetFloat("k_env_specular", m_Env_Specular);
            composite_shader->SetFloat("k_voxelgi", m_VoxelGI);
            composite_shader->SetFloat("k_ssr_factor", m_SSR_Factor);

            composite_shader->SetInt("g_DirectLightinging_diffuse", 0);
            m_FrameRenderBuffer_DirectLighting_diffuse->BindTexture(0);
            composite_shader->SetInt("g_DirectLightinging_specular", 1);
            m_FrameRenderBuffer_DirectLighting_specular->BindTexture(1);
            composite_shader->SetInt("g_EnvironmentLighting_diffuse", 2);
            m_FrameRenderBuffer_EnvironmentLighting_diffuse->BindTexture(2);
            composite_shader->SetInt("g_EnvironmentLighting_specular", 3);
            m_FrameRenderBuffer_EnvironmentLighting_specular->BindTexture(3);
            composite_shader->SetInt("g_Skybox", 4);
            m_FrameRenderBuffer_skybox->BindTexture(4);
            composite_shader->SetInt("g_ScreenSpaceReflection", 5);
            m_FrameRenderBuffer_ssr_blur->BindTexture(5);
            composite_shader->SetInt("g_AO", 6);
            m_SSAOBuffer->BindSSAOTexture(6);
            composite_shader->SetInt("g_VoxelGI", 7);
            m_FrameRenderBuffer_VoxelGI->BindTexture(7);

            RenderCommand::RenderToQuad();

            m_SSAOBuffer->UnBindTexture(6);
            m_FrameRenderBuffer_ssr_blur->UnBindTexture(5);
            m_FrameRenderBuffer_skybox->UnBindTexture(4);
            m_FrameRenderBuffer_EnvironmentLighting_specular->UnBindTexture(3);
            m_FrameRenderBuffer_EnvironmentLighting_diffuse->UnBindTexture(2);
            m_FrameRenderBuffer_DirectLighting_specular->UnBindTexture(1);
            m_FrameRenderBuffer_DirectLighting_diffuse->UnBindTexture(0);

            composite_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer);
            glEnable(GL_DEPTH_TEST);
        }

        // extract highLight
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_highLight->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto highLight_shader = m_ShaderLibrary.Get("HighLight");
            highLight_shader->Bind();

            highLight_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer->BindTexture(0);

            RenderCommand::RenderToQuad();

            m_FrameRenderBuffer->UnBindTexture(0);

            highLight_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_highLight);
            glEnable(GL_DEPTH_TEST);
        }

        // downsample to m_FrameRenderBuffer_highLight_downSampled
        for (int i = 0; i < 7; i++)
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_highLight_downSampled[i]->Bind();
            RenderCommand::SetViewPort(0,
                                       0,
                                       m_FrameRenderBuffer_highLight_downSampled[i]->GetWidth(),
                                       m_FrameRenderBuffer_highLight_downSampled[i]->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto downsample_shader = m_ShaderLibrary.Get("Identity");
            downsample_shader->Bind();

            downsample_shader->SetInt("g_Color", 0);
            if (i == 0)
            {
                m_FrameRenderBuffer_highLight->BindTexture(0);
            }
            else
            {
                m_FrameRenderBuffer_highLight_downSampled[i - 1]->BindTexture(0);
            }

            RenderCommand::RenderToQuad();

            if (i == 0)
            {
                m_FrameRenderBuffer_highLight->UnBindTexture(0);
            }
            else
            {
                m_FrameRenderBuffer_highLight_downSampled[i - 1]->UnBindTexture(0);
            }

            downsample_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_highLight_downSampled[i]);
            glEnable(GL_DEPTH_TEST);
        }
        // blur
        for (int i = 0; i < 6; i++)
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_highLight_blur[i]->Bind();
            RenderCommand::SetViewPort(0,
                                       0,
                                       m_FrameRenderBuffer_highLight_blur[i]->GetWidth(),
                                       m_FrameRenderBuffer_highLight_blur[i]->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto blur_shader = m_ShaderLibrary.Get("GaussianBlur");
            blur_shader->Bind();

            blur_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_highLight_downSampled[i]->BindTexture(0);
            blur_shader->SetFloat2("screenSize",
                                   glm::vec2(m_FrameRenderBuffer_highLight_blur[i]->GetWidth(),
                                             m_FrameRenderBuffer_highLight_blur[i]->GetHeight()) /
                                       1.0f);

            RenderCommand::RenderToQuad();

            m_FrameRenderBuffer_highLight_downSampled[i]->UnBindTexture(0);

            blur_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_highLight_blur[i]);
            glEnable(GL_DEPTH_TEST);
        }
        // upsample and addition
        for (int i = 5; i >= 0; --i)
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_highLight_upSampled[i]->Bind();
            RenderCommand::SetViewPort(0,
                                       0,
                                       m_FrameRenderBuffer_highLight_upSampled[i]->GetWidth(),
                                       m_FrameRenderBuffer_highLight_upSampled[i]->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto upsample_shader = m_ShaderLibrary.Get("Addition");
            upsample_shader->Bind();

            upsample_shader->SetInt("g_ImageA", 0);
            if (i == 5)
            {
                m_FrameRenderBuffer_highLight_downSampled[6]->BindTexture(0);
            }
            else
            {
                m_FrameRenderBuffer_highLight_upSampled[i + 1]->BindTexture(0);
            }
            upsample_shader->SetInt("g_ImageB", 1);
            upsample_shader->SetFloat("weight", 1.0f);
            m_FrameRenderBuffer_highLight_blur[i]->BindTexture(1);

            RenderCommand::RenderToQuad();

            m_FrameRenderBuffer_highLight_blur[i]->UnBindTexture(1);
            if (i == 5)
            {
                m_FrameRenderBuffer_highLight_downSampled[6]->UnBindTexture(0);
            }
            else
            {
                m_FrameRenderBuffer_highLight_upSampled[i + 1]->UnBindTexture(0);
            }

            upsample_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_highLight_upSampled[i]);
            glEnable(GL_DEPTH_TEST);
        }

        // add to orgin
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_bloom->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto addition_shader = m_ShaderLibrary.Get("Addition");
            addition_shader->Bind();

            addition_shader->SetInt("g_ImageA", 0);
            m_FrameRenderBuffer->BindTexture(0);

            addition_shader->SetInt("g_ImageB", 1);
            m_FrameRenderBuffer_highLight_upSampled[0]->BindTexture(1);

            addition_shader->SetFloat("weight", m_Bloom_Intensity);

            RenderCommand::RenderToQuad();

            m_FrameRenderBuffer_highLight_upSampled[0]->UnBindTexture(1);
            m_FrameRenderBuffer->UnBindTexture(0);

            addition_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_bloom);
            glEnable(GL_DEPTH_TEST);
        }

        // exposure
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_exposure->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto exposure_shader = m_ShaderLibrary.Get("Exposure");
            exposure_shader->Bind();

            exposure_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_bloom->BindTexture(0);
            exposure_shader->SetFloat("exposure", m_Exposure);

            RenderCommand::RenderToQuad();

            m_FrameRenderBuffer_bloom->UnBindTexture(0);

            exposure_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_exposure);
            glEnable(GL_DEPTH_TEST);
        }

        // FXAA
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_fxaa->Bind();
            RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto fxaa_shader = m_ShaderLibrary.Get("FXAA");
            fxaa_shader->Bind();

            fxaa_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_exposure->BindTexture(0);
            fxaa_shader->SetFloat2("resolution",
                                   glm::vec2(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight()));

            RenderCommand::RenderToQuad();

            m_FrameRenderBuffer_exposure->UnBindTexture(0);

            fxaa_shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_fxaa);
            glEnable(GL_DEPTH_TEST);
        }

        // forward
        // Point Cloud
        m_FrameRenderBuffer->Bind();

        for (auto [entity, name, trans, pointcloud] : pointcloud_view.each())
        {
            APointCloud& pointcloud_actor = *static_cast<APointCloud*>(pointcloud.GetOwner());

            if (pointcloud_actor.GetVisible() == false)
            {
                continue;
            }

            auto shader = m_ShaderLibrary.Get("OctreeShader");
            shader->Bind();

            shader->SetFloat("u_PointRenderSize", pointcloud.GetPointRenderSize());
            Renderer::DrawArray(pointcloud.GetPointCloud().m_VertexArray, shader, m_VPMatrix, trans.GetTransform());

            shader->UnBind();
        }

        // draw skeleton
        for (auto [entity, name, trans, skeleton] : skeleton_view.each())
        {
            ASkeleton& skeleton_actor = *static_cast<ASkeleton*>(skeleton.GetOwner());

            if (skeleton_actor.GetVisible() == false)
            {
                continue;
            }

            if (skeleton.GetShowSkeleton() == false)
            {
                continue;
            }

            auto shader = m_ShaderLibrary.Get("Skeleton");
            skeleton.Draw(shader,
                          m_VPMatrix,
                          trans.GetTransform(),
                          actor_mainCamera->GetTransformComponent().GetPosition(),
                          glm::vec3(0.0f),
                          glm::vec3(1.0f));
        }

        // draw skeleton
        for (auto [entity, name, trans, skinnedmesh] : skinnedmesh_view.each())
        {
            ASkinnedMesh& skinnedmesh_actor = *static_cast<ASkinnedMesh*>(skinnedmesh.GetOwner());

            if (skinnedmesh_actor.GetVisible() == false)
            {
                continue;
            }

            auto skeletonShader = m_ShaderLibrary.Get("Skeleton");
            skeletonShader->Bind();

            skinnedmesh.DrawSkeleton(skeletonShader,
                                     m_VPMatrix,
                                     trans.GetTransform(),
                                     actor_mainCamera->GetTransformComponent().GetPosition(),
                                     glm::vec3(0.0f),
                                     glm::vec3(1.0f));

            skeletonShader->UnBind();
        }

        glDisable(GL_DEPTH_TEST);
        // draw pawn
        for (auto [entity, name, trans, pawn] : pawn_view.each())
        {
            auto shader = m_ShaderLibrary.Get("Pawn");
            pawn.Draw(shader, m_VPMatrix, trans.GetTransform());
        }

        // draw trajectory
        for (auto [entity, name, trans, trajectory] : trajectory_view.each())
        {
            auto shader = m_ShaderLibrary.Get("Trajectory");
            trajectory.Draw(shader, m_VPMatrix, trans.GetTransform());
        }

        // draw motionmatching
        for (auto [entity, name, trans, motionmatching] : motionmatching_view.each())
        {
            auto shader = m_ShaderLibrary.Get("Trajectory");
            motionmatching.DrawTrajectory(shader, m_VPMatrix, trans.GetTransform());
        }

        glEnable(GL_DEPTH_TEST);

        Renderer::EndScene(m_FrameRenderBuffer);

        // Render to VoxelGIViewport
        if (viewport_items[m_ViewportGBufferMap] == "VoxelGI_VoxelTexture")
        {
            glDisable(GL_DEPTH_TEST);
            m_FrameRenderBuffer_VoxelGIViewport->Bind();
            RenderCommand::SetViewPort(0,
                                       0,
                                       m_FrameRenderBuffer_VoxelGIViewport->GetWidth(),
                                       m_FrameRenderBuffer_VoxelGIViewport->GetHeight());

            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            RenderCommand::Clear();

            auto shader = m_ShaderLibrary.Get("VoxelGI_Visualization");
            shader->Bind();

            shader->SetInt("g_VoxelTexture", 0);
            m_VoxelGI_VoxelTexture->Bind(0);

            shader->SetInt("g_WorldPosition", 1);
            m_GeometryBuffer->BindWorldPositionTexture(1);

            shader->SetFloat3("u_scene_voxel_scale", m_VoxelGI_settings.scene_voxel_scale);
            shader->SetFloat3("u_CameraPosition", actor_mainCamera->GetTransformComponent().GetPosition());

            RenderCommand::RenderToQuad();

            m_GeometryBuffer->UnBindTexture(1);
            m_VoxelGI_VoxelTexture->UnBind(0);

            shader->UnBind();
            Renderer::EndScene(m_FrameRenderBuffer_VoxelGIViewport);
            glEnable(GL_DEPTH_TEST);
        }

        // Render GBuffer Viewport
        m_FrameRenderBuffer_bufferViewport->Bind();
        RenderCommand::SetViewPort(
            0, 0, m_FrameRenderBuffer_bufferViewport->GetWidth(), m_FrameRenderBuffer_bufferViewport->GetHeight());

        RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        RenderCommand::Clear();

        std::string gbuffer_shader_name[] = {
            "VisPosition", "VisNormal", "Screen",      "VisDepth", "VisAO",  "VisRoughness", "Screen",
            "VisPosition", "VisNormal", "VisEmissive", "Screen",   "Screen", "Screen",       "Screen",
            "Screen",      "Screen",    "Screen",      "Screen",   "Screen", "Screen",       "Screen",
        };

        const std::string viewport_buffer_name    = viewport_items[m_ViewportGBufferMap];
        auto              gbuffer_viewport_shader = m_ShaderLibrary.Get(gbuffer_shader_name[m_ViewportGBufferMap]);

        gbuffer_viewport_shader->Bind();

        if (viewport_buffer_name == "ViewPosition")
        {
            gbuffer_viewport_shader->SetInt("g_Position", 0);
            m_GeometryBuffer->BindViewPositionTexture(0);
        }
        if (viewport_buffer_name == "ViewNormal")
        {
            gbuffer_viewport_shader->SetInt("g_Normal", 0);
            m_GeometryBuffer->BindViewNormalTexture(0);
        }
        if (viewport_buffer_name == "Diffuse")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_GeometryBuffer->BindDiffuseTexture(0);
        }
        if (viewport_buffer_name == "Depth")
        {
            gbuffer_viewport_shader->SetInt("g_Depth", 0);
            m_GeometryBuffer->BindDepthTexture(0);
        }
        if (viewport_buffer_name == "Ambient Occlusion")
        {
            gbuffer_viewport_shader->SetInt("g_AO", 0);
            m_SSAOBuffer->BindSSAOTexture(0);
        }
        if (viewport_buffer_name == "Roughness")
        {
            gbuffer_viewport_shader->SetInt("g_Roughness", 0);
            m_GeometryBuffer->BindRoughnessTexture(0);
        }
        if (viewport_buffer_name == "Specular")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_GeometryBuffer->BindSpecularTexture(0);
        }
        if (viewport_buffer_name == "WorldPosition")
        {
            gbuffer_viewport_shader->SetInt("g_Position", 0);
            m_GeometryBuffer->BindWorldPositionTexture(0);
        }
        if (viewport_buffer_name == "WorldNormal")
        {
            gbuffer_viewport_shader->SetInt("g_Normal", 0);
            m_GeometryBuffer->BindWorldNormalTexture(0);
        }
        if (viewport_buffer_name == "Emissive")
        {
            gbuffer_viewport_shader->SetInt("g_Emissive", 0);
            m_GeometryBuffer->BindSpecularTexture(0);
        }
        if (viewport_buffer_name == "Skybox")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_skybox->BindTexture(0);
        }
        if (viewport_buffer_name == "DirectLighting_Diffuse")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_DirectLighting_diffuse->BindTexture(0);
        }
        if (viewport_buffer_name == "DirectLighting_Specular")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_DirectLighting_specular->BindTexture(0);
        }
        if (viewport_buffer_name == "EnvironmentLighting_Diffuse")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_EnvironmentLighting_diffuse->BindTexture(0);
        }
        if (viewport_buffer_name == "EnvironmentLighting_Specular")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_EnvironmentLighting_specular->BindTexture(0);
        }
        if (viewport_buffer_name == "ScreenSpaceReflection")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_ssr->BindTexture(0);
        }
        if (viewport_buffer_name == "ScreenSpaceReflection_Blur")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_ssr_blur->BindTexture(0);
        }
        if (viewport_buffer_name == "Bloom")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_highLight_upSampled[0]->BindTexture(0);
        }
        if (viewport_buffer_name == "Exposure")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_exposure->BindTexture(0);
        }
        if (viewport_buffer_name == "VoxelGI_VoxelTexture")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_VoxelGIViewport->BindTexture(0);
        }
        if (viewport_buffer_name == "VoxelGI")
        {
            gbuffer_viewport_shader->SetInt("g_Color", 0);
            m_FrameRenderBuffer_VoxelGI->BindTexture(0);
        }

        RenderCommand::RenderToQuad();

        m_GeometryBuffer->UnBindTexture(0);
        gbuffer_viewport_shader->UnBind();

        Renderer::EndScene(m_FrameRenderBuffer_bufferViewport);

        // Render to Screen
        glDisable(GL_DEPTH_TEST);
        m_FrameRenderBuffer->Bind();
        RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        RenderCommand::Clear();

        auto screen_shader = m_ShaderLibrary.Get("Screen");
        screen_shader->Bind();

        screen_shader->SetInt("g_Color", 0);
        m_FrameRenderBuffer_fxaa->BindTexture(0);

        RenderCommand::RenderToQuad();

        m_FrameRenderBuffer_fxaa->UnBindTexture(0);

        screen_shader->UnBind();
        Renderer::EndScene(m_FrameRenderBuffer);
        glEnable(GL_DEPTH_TEST);
    }

    void UWorld::TickGui(float timeStep)
    {
        // Render Setting
        Gui::Begin("Render Setting");

        Gui::Text("General");
        Gui::SliderFloat("Exposure", m_Exposure, 0.0f, 3.0f);
        Gui::ColorEdit4("Background Color", m_BackGroundColor);
        ImGui::Checkbox("Render Skybox", &m_RenderSkybox);
        ImGui::Checkbox("Render Grid", &m_RenderGrid);
        ImGui::Checkbox("Render Gizmo", &m_RenderGizmo);
        ImGui::Combo("Viewport Map", &m_ViewportGBufferMap, viewport_items, IM_ARRAYSIZE(viewport_items));

        ImGui::SliderFloat("Direct Diffuse", &m_Dir_Diffuse, 0.0f, 5.0f);
        ImGui::SliderFloat("Direct Specular", &m_Dir_Specular, 0.0f, 5.0f);
        ImGui::SliderFloat("Environment Diffuse", &m_Env_Diffuse, 0.0f, 5.0f);
        ImGui::SliderFloat("Environment Specular", &m_Env_Specular, 0.0f, 5.0f);
        ImGui::SliderFloat("VoxelGI", &m_VoxelGI, 0.0f, 5.0f);
        ImGui::SliderFloat("SSR Factor", &m_SSR_Factor, 0.0f, 5.0f);

        ImGui::Separator();

        Gui::Text("Voxel-Based Global Illumination (VXGI)");
        ImGui::SliderInt("Voxel Grid Resolution", &m_VoxelGI_settings.voxel_grid_resolution, 1, 256);
        ImGui::SliderInt("Max Mipmap Level", &m_VoxelGI_settings.max_mipmap_level, 1, 10);
        ImGui::SliderFloat3("Scene Voxel Scale", &m_VoxelGI_settings.scene_voxel_scale[0], 0.05f, 1.0f);
        ImGui::SliderFloat("Diffuse Aperture", &m_VoxelGI_settings.diffuse.aperture, 00.05f, 1.0f);
        ImGui::SliderFloat("Diffuse Sampling Factor", &m_VoxelGI_settings.diffuse.sampling_factor, 0.05f, 1.0f);
        ImGui::SliderFloat("Diffuse Distance Offset", &m_VoxelGI_settings.diffuse.distance_offset, 0.05f, 1.0f);
        ImGui::SliderFloat("Diffuse Max Distance", &m_VoxelGI_settings.diffuse.max_distance, 0.05f, 5.0f);
        ImGui::SliderFloat("Diffuse Result Intensity", &m_VoxelGI_settings.diffuse.result_intensity, 0.05f, 10.0f);
        ImGui::SliderFloat("Specular Aperture", &m_VoxelGI_settings.specular.aperture, 0.05f, 1.0f);
        ImGui::SliderFloat("Specular Sampling Factor", &m_VoxelGI_settings.specular.sampling_factor, 0.05f, 1.0f);
        ImGui::SliderFloat("Specular Distance Offset", &m_VoxelGI_settings.specular.distance_offset, 0.05f, 1.0f);
        ImGui::SliderFloat("Specular Max Distance", &m_VoxelGI_settings.specular.max_distance, 0.05f, 5.0f);
        ImGui::SliderFloat("Specular Result Intensity", &m_VoxelGI_settings.specular.result_intensity, 0.05f, 10.0f);

        ImGui::Separator();

        Gui::Text("Percentage-Closer Soft Shadows (PCSS)");
        ImGui::SliderFloat("FilterRadius", &m_PCSS_FilterRadius, 0.0f, 30.0f);

        ImGui::Separator();

        Gui::Text("Bloom");
        Gui::SliderFloat("Intensity", m_Bloom_Intensity, 0.0f, 1.0f);

        ImGui::Separator();

        Gui::Text("Screen Space Ambient Occlusion (SSAO)");
        ImGui::SliderFloat("Radius", &m_SSAOBuffer->GetRadiusRef(), 0.05f, 2.0f);
        ImGui::SliderFloat("Bias", &m_SSAOBuffer->GetBiasRef(), 0.0f, 0.005f);
        ImGui::SliderFloat("Power", &m_SSAOBuffer->GetPowerRef(), 0.0f, 5.0f);

        ImGui::Separator();

        Gui::Text("Screen Space Reflection (SSR)");
        ImGui::Checkbox("Debug", &m_SSR_settings.debug);
        ImGui::SliderFloat("Ray Step", &m_SSR_settings.rayStep, 0.001f, 1.0f);
        ImGui::SliderFloat("Min Ray Step", &m_SSR_settings.minRayStep, 0.001f, 1.0f);
        ImGui::SliderFloat("Max Steps", &m_SSR_settings.maxSteps, 1.0f, 1800.0f);
        ImGui::SliderInt("Num Binary Search Steps", &m_SSR_settings.numBinarySearchSteps, 1, 20);
        ImGui::SliderFloat(
            "Reflection Specular Falloff Exponent", &m_SSR_settings.reflectionSpecularFalloffExponent, 0.0f, 10.0f);
        ImGui::SliderFloat("Ref Bias", &m_SSR_settings.refBias, -10.0f, 10.0f);

        Gui::End();

        // Scence Collection
        Gui::Begin("Scence Collection");

        auto object_view          = m_Registry.view<UTagComponent, UTransformComponent>();
        auto camrea_view          = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto staticmodel_view     = m_Registry.view<UTagComponent, UTransformComponent, UStaticMeshComponent>();
        auto pointcloud_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointCloudComponent>();
        auto pointlight_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();
        auto dirctionallight_view = m_Registry.view<UTagComponent, UTransformComponent, UDirectionalLightComponent>();
        auto skybox_view          = m_Registry.view<UTagComponent, UTransformComponent, USkyboxComponent>();
        auto skeleton_view        = m_Registry.view<UTagComponent, UTransformComponent, USkeletonComponent>();
        auto skinnedmesh_view     = m_Registry.view<UTagComponent, UTransformComponent, USkinnedMeshComponent>();

        static ImGuiTreeNodeFlags base_flags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

        int index = 0;

        for (auto [entity, name, trans] : object_view.each())
        {
            ImGuiTreeNodeFlags node_flags = base_flags;

            const bool is_selected = entity == entity_selected;
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;

            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)index, node_flags, name.GetString().c_str());
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                entity_selected = entity;
            }

            if (node_open)
            {
                ImGui::TreePop();
            }

            index = index + 1;
        }

        Gui::End();

        Gui::Begin("Details");

        if (entity_selected != entt::null && m_Registry.valid(entity_selected) == true)
        {
            AActor* actor = static_cast<AActor*>(m_Registry.get<UTagComponent>(entity_selected).GetOwner());

            if (m_Registry.any_of<UTagComponent>(entity_selected) == true)
            {
                auto& tagComponent = m_Registry.get<UTagComponent>(entity_selected);
                Gui::Text(tagComponent.GetString().c_str());
                ImGui::Separator();

                if (ImGui::Button("Control the Actor"))
                {
                    m_ControlledActor->SetIsControlled(false);
                    m_ControlledActor = actor;
                    m_ControlledActor->SetIsControlled(true);
                }

                ImGui::BeginDisabled();
                ImGui::Checkbox("Is Controlled", &actor->GetIsControlledRef());
                ImGui::EndDisabled();

                ImGui::Checkbox("Visible", &actor->GetVisibleRef());
            }

            if (m_Registry.any_of<UTransformComponent>(entity_selected) == true)
            {
                auto& tansformComponent = m_Registry.get<UTransformComponent>(entity_selected);
                Gui::Text("Transform");
                ImGui::Separator();

                Gui::DragFloat3("Position", tansformComponent.GetPositionRef(), 0.005f, -100.0f, 100.0f);
                Gui::DragFloat3("Rotation", tansformComponent.GetRotationRef(), 0.005f, -100.0f, 100.0f);
                Gui::DragFloat3("Scale", tansformComponent.GetScaleRef(), 0.005f, 0.0f, 100.0f);
            }

            if (m_Registry.any_of<UCameraComponent>(entity_selected) == true)
            {
                auto& cameraComponent = m_Registry.get<UCameraComponent>(entity_selected);
                auto& camera          = cameraComponent.GetCamera();
                Gui::Text("Camera");
                ImGui::Separator();

                std::string cameraType = camera.GetCameraType();
                if (cameraType == "PerspectiveCamera")
                {
                    auto& perspectiveCamera = reinterpret_cast<PerspectiveCamera&>(camera);

                    Gui::Text(fmt::format("Camera Type: {}", cameraType));
                    if (ImGui::Button("Set Viewport Camera"))
                    {
                        m_MainCamera->SetIsViewportCamera(false);
                        m_MainCamera = &cameraComponent;
                        m_MainCamera->SetIsViewportCamera(true);
                    }
                    Gui::Text(fmt::format("Aspect: {0}", perspectiveCamera.GetAspectRatioRef()));

                    ImGui::BeginDisabled();
                    ImGui::Checkbox("Viewport Camera", &cameraComponent.GetIsViewportCameraRef());
                    ImGui::EndDisabled();

                    Gui::SliderFloat("Fov", perspectiveCamera.GetFovRef(), 1.8f, 160.0f);
                    Gui::SliderFloat("Near", perspectiveCamera.GetNearClipRef(), 0.01f, 100.0f);
                    Gui::SliderFloat("Far", perspectiveCamera.GetFarClipRef(), 0.3f, 10000.0f);
                }

                Gui::DragFloat3("Cam::Position", cameraComponent.GetPositionRef(), 0.005f, -100.0f, 100.0f);
                Gui::DragFloat3("Cam::Rotation", cameraComponent.GetRotationRef(), 0.005f, -100.0f, 100.0f);
                Gui::DragFloat3("Cam::Scale", cameraComponent.GetScaleRef(), 0.005f, 0.0f, 100.0f);
            }

            if (m_Registry.any_of<UStaticMeshComponent>(entity_selected) == true)
            {
                auto& staticMeshComponent = m_Registry.get<UStaticMeshComponent>(entity_selected);
                Gui::Text("Static Mesh");
                ImGui::Separator();

                AStaticMesh* staticMesh_actor = static_cast<AStaticMesh*>(staticMeshComponent.GetOwner());
                MMaterial*   material         = static_cast<MMaterial*>(staticMesh_actor->GetMaterial());

                // Material
                std::string materialType = material->GetMaterialType();
                if (materialType == "BasicPbr")
                {
                    MBasicPbr* material_basicPbr = static_cast<MBasicPbr*>(material);
                    Gui::Text("Material Type: BasicPbr");
                    ImGui::ColorEdit3("Albedo", glm::value_ptr(material_basicPbr->GetAlbedoRef()));
                    Gui::SliderFloat("Metallic", material_basicPbr->GetMetallicRef(), 0.0f, 1.0f);
                    Gui::SliderFloat("Roughness", material_basicPbr->GetRoughnessRef(), 0.0f, 1.0f);
                    Gui::SliderFloat("AO", material_basicPbr->GetAORef(), 0.0f, 1.0f);
                }
                else if (materialType == "TriangleShader")
                {
                    MTriangleShader* material_triangleShader = static_cast<MTriangleShader*>(material);
                    Gui::Text("Material Type: TriangleShader");
                    ImGui::ColorEdit3("Color", glm::value_ptr(material_triangleShader->GetColorRef()));
                }
            }

            if (m_Registry.any_of<UPointCloudComponent>(entity_selected) == true)
            {
                auto& pointCloudComponent = m_Registry.get<UPointCloudComponent>(entity_selected);
                Gui::Text("Point Cloud");
                ImGui::Separator();

                Gui::Text("FilePath:");
                ImGui::TextWrapped("%s", pointCloudComponent.GetFilePath().c_str());

                Gui::Text(fmt::format("Num Points: {}", pointCloudComponent.GetNumPoints()));
                Gui::SliderFloat("PointRenderSize", pointCloudComponent.GetPointRenderSizeRef(), 0.0f, 100.0f);
            }

            if (m_Registry.any_of<UPointLightComponent>(entity_selected) == true)
            {
                auto& pointLightComponent = m_Registry.get<UPointLightComponent>(entity_selected);
                Gui::Text("Point Light");
                ImGui::Separator();

                ImGui::ColorEdit3("Color", glm::value_ptr(pointLightComponent.GetColorRef()));
                Gui::SliderFloat("Intensity", pointLightComponent.GetIntensityRef(), 0.0f, 30.0f);

                // Render texture to ShadowCube Viewport
                ShadowCubeMapBuffer& shadowCubeMapBuffer = pointLightComponent.GetShadowCubeMapBufferRef();
                m_FrameRenderBuffer_shadowCubeMapViewport->Bind();
                RenderCommand::SetViewPort(0, 0, 512, 256);
                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                auto shader = m_ShaderLibrary.Get("VisCubeDepth");
                shader->Bind();

                shader->SetInt("g_Depth", 0);
                shadowCubeMapBuffer.BindTexture(0);

                shader->SetFloat("u_NearPlane", 0.1f);
                shader->SetFloat("u_FarPlane", 33.0f);

                RenderCommand::RenderToQuad();

                shadowCubeMapBuffer.UnBindTexture(0);

                shader->UnBind();
                m_FrameRenderBuffer_shadowCubeMapViewport->UnBind();

                ImGui::Text("ShadowCubeMap");

                ImGuiIO& io         = ImGui::GetIO();
                ImVec2   pos        = ImGui::GetCursorScreenPos();
                int      my_tex_w   = 512;
                int      my_tex_h   = 256;
                ImVec4   tint_col   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                ImVec4   border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

                ImGui::Image(m_FrameRenderBuffer_shadowCubeMapViewport->GetTextureID(),
                             ImVec2(512, 256),
                             ImVec2(0, 1),
                             ImVec2(1, 0),
                             tint_col,
                             border_col);

                if (ImGui::BeginItemTooltip())
                {
                    float region_sz = 32.0f;
                    float region_x  = io.MousePos.x - pos.x - region_sz * 0.5f;
                    float region_y  = io.MousePos.y - pos.y - region_sz * 0.5f;
                    float zoom      = 4.0f;
                    if (region_x < 0.0f)
                    {
                        region_x = 0.0f;
                    }
                    else if (region_x > my_tex_w - region_sz)
                    {
                        region_x = my_tex_w - region_sz;
                    }
                    if (region_y < 0.0f)
                    {
                        region_y = 0.0f;
                    }
                    else if (region_y > my_tex_h - region_sz)
                    {
                        region_y = my_tex_h - region_sz;
                    }
                    ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                    ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                    ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                    uv0.y      = 1.0f - uv0.y;
                    ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                    uv1.y      = 1.0f - uv1.y;
                    ImGui::Image(m_FrameRenderBuffer_shadowCubeMapViewport->GetTextureID(),
                                 ImVec2(region_sz * zoom, region_sz * zoom),
                                 uv0,
                                 uv1,
                                 tint_col,
                                 border_col);
                    ImGui::EndTooltip();
                }
            }

            if (m_Registry.any_of<UDirectionalLightComponent>(entity_selected) == true)
            {
                auto& directionalLightComponent = m_Registry.get<UDirectionalLightComponent>(entity_selected);
                Gui::Text("Directional Light");
                ImGui::Separator();

                ImGui::ColorEdit3("Color", glm::value_ptr(directionalLightComponent.GetColorRef()));
                {
                    glm::vec3 direction = directionalLightComponent.GetDirectionRef();
                    Gui::DragFloat3("Direction", direction, 0.005f, -1.0f, 1.0f);
                    directionalLightComponent.SetDirection(direction);
                }
                Gui::SliderFloat("Intensity", directionalLightComponent.GetIntensityRef(), 0.0f, 30.0f);

                // Render texture to ShadowMap Viewport
                ShadowMapBuffer& shadowMapBuffer = directionalLightComponent.GetShadowMapBufferRef();
                m_FrameRenderBuffer_shadowMapViewport->Bind();
                RenderCommand::SetViewPort(0, 0, 256, 256);
                RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                RenderCommand::Clear();

                auto shader = m_ShaderLibrary.Get("VisDepth");
                shader->Bind();

                shader->SetInt("g_Depth", 0);
                shadowMapBuffer.BindTexture(0);

                RenderCommand::RenderToQuad();

                shadowMapBuffer.UnBindTexture(0);

                shader->UnBind();
                m_FrameRenderBuffer_shadowMapViewport->UnBind();

                ImGui::Text("ShadowMap");

                ImGuiIO& io         = ImGui::GetIO();
                ImVec2   pos        = ImGui::GetCursorScreenPos();
                int      my_tex_w   = 256;
                int      my_tex_h   = 256;
                ImVec4   tint_col   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                ImVec4   border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

                ImGui::Image(m_FrameRenderBuffer_shadowMapViewport->GetTextureID(),
                             ImVec2(256, 256),
                             ImVec2(0, 1),
                             ImVec2(1, 0),
                             tint_col,
                             border_col);

                if (ImGui::BeginItemTooltip())
                {
                    float region_sz = 32.0f;
                    float region_x  = io.MousePos.x - pos.x - region_sz * 0.5f;
                    float region_y  = io.MousePos.y - pos.y - region_sz * 0.5f;
                    float zoom      = 4.0f;
                    if (region_x < 0.0f)
                    {
                        region_x = 0.0f;
                    }
                    else if (region_x > my_tex_w - region_sz)
                    {
                        region_x = my_tex_w - region_sz;
                    }
                    if (region_y < 0.0f)
                    {
                        region_y = 0.0f;
                    }
                    else if (region_y > my_tex_h - region_sz)
                    {
                        region_y = my_tex_h - region_sz;
                    }
                    ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                    ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                    ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                    uv0.y      = 1.0f - uv0.y;
                    ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                    uv1.y      = 1.0f - uv1.y;
                    ImGui::Image(m_FrameRenderBuffer_shadowMapViewport->GetTextureID(),
                                 ImVec2(region_sz * zoom, region_sz * zoom),
                                 uv0,
                                 uv1,
                                 tint_col,
                                 border_col);
                    ImGui::EndTooltip();
                }
            }

            if (m_Registry.any_of<USkyboxComponent>(entity_selected) == true)
            {
                auto& skyboxComponent = m_Registry.get<USkyboxComponent>(entity_selected);
                Gui::Text("Skybox");
                ImGui::Separator();

                // Skybox
                // use imgui add a bool selector

                const char* items[] = {"CubeMap", "Irradiance", "Prefilter"};
                ImGui::Combo("Show Map", &skyboxComponent.GetCruuentTextureIndexRef(), items, IM_ARRAYSIZE(items));

                Gui::SliderFloat("Mipmap Level", m_VisPrePrefilterMipLevel, 0.0f, 32.0f);
            }

            if (m_Registry.any_of<USkeletonComponent>(entity_selected) == true)
            {
                auto& skeletonComponent = m_Registry.get<USkeletonComponent>(entity_selected);
                Gui::Text("Skeleton");
                ImGui::Separator();

                Gui::Text(fmt::format("Joints Num: {0}", skeletonComponent.GetNumJoints()));

                ImGui::Checkbox("Render Skeleton", &skeletonComponent.GetShowSkeletonRef());
            }

            if (m_Registry.any_of<USkinnedMeshComponent>(entity_selected) == true)
            {
                auto& skinnedMeshComponent = m_Registry.get<USkinnedMeshComponent>(entity_selected);
                Gui::Text("Skinned Mesh");
                ImGui::Separator();

                Gui::Text(
                    fmt::format("Num Joints: {0}", skinnedMeshComponent.GetSkeletonComponentRef().GetNumJoints()));

                ImGui::Checkbox("Render Skeleton",
                                &skinnedMeshComponent.GetSkeletonComponentRef().GetShowSkeletonRef());
                ImGui::Checkbox("Render Skinned Mesh", &skinnedMeshComponent.GetShowSkinnedMeshRef());

                ASkinnedMesh& skinnedmesh_actor = *static_cast<ASkinnedMesh*>(skinnedMeshComponent.GetOwner());
                MMaterial*    material          = static_cast<MMaterial*>(skinnedMeshComponent.GetMaterial());

                // Material
                std::string materialType = material->GetMaterialType();
                if (materialType == "BasicPbr")
                {
                    MBasicPbr* material_basicPbr = static_cast<MBasicPbr*>(material);
                    Gui::Text("Material Type: BasicPbr");
                    ImGui::ColorEdit3("Albedo", glm::value_ptr(material_basicPbr->GetAlbedoRef()));
                    Gui::SliderFloat("Metallic", material_basicPbr->GetMetallicRef(), 0.0f, 1.0f);
                    Gui::SliderFloat("Roughness", material_basicPbr->GetRoughnessRef(), 0.0f, 1.0f);
                    Gui::SliderFloat("AO", material_basicPbr->GetAORef(), 0.0f, 1.0f);
                }
                else if (materialType == "TriangleShader")
                {
                    MTriangleShader* material_triangleShader = static_cast<MTriangleShader*>(material);
                    Gui::Text("Material Type: TriangleShader");
                    ImGui::ColorEdit3("Color", glm::value_ptr(material_triangleShader->GetColorRef()));
                }
            }

            if (m_Registry.any_of<UPawnComponent>(entity_selected) == true)
            {
                auto& pawnComponent = m_Registry.get<UPawnComponent>(entity_selected);
                Gui::Text("Pawn");
                ImGui::Separator();

                Gui::SliderFloat("CameraDistance", pawnComponent.GetCameraDistanceRef(), 0.0f, 10.0f);
                Gui::SliderFloat("CameraLongitude", pawnComponent.GetCameraLongitudeRef(), -3.0f, 3.0f);
                Gui::SliderFloat("CameraLatitude", pawnComponent.GetCameraLatitudeRef(), -3.0f, 3.0f);

                Gui::SliderFloat3("CameraLookAt", pawnComponent.GetCameraLookAtRef(), -10.0f, 10.0f);

                Gui::SliderFloat("PawnMoveSpeed", pawnComponent.GetPawnMoveSpeedRef(), 0.0f, 10.0f);
                Gui::SliderFloat("MouseSensitivityX", pawnComponent.GetMouseSensitivityXRef(), 0.0f, 0.1f);
                Gui::SliderFloat("MouseSensitivityY", pawnComponent.GetMouseSensitivityYRef(), 0.0f, 0.1f);
            }
        }

        Gui::End();
    }
}; // namespace Engine