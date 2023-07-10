#include <Engine/Runtime/GameFramework/Common/UWorld.h>

#include <memory>

#include <Engine/Runtime/Renderer/Shader.h>
#include <Engine/Runtime/Renderer/Buffer.h>
#include <Engine/Runtime/Gui/GuiCommand.h>
#include <Engine/Runtime/Core/Input/Input.h>

#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/GameFramework/Camera/UCameraComponent.h>
#include <Engine/Runtime/GameFramework/StaticMesh/AStaticMesh.h>
#include <Engine/Runtime/GameFramework/StaticMesh/APointCloud.h>
#include <Engine/Runtime/GameFramework/Animation/ASkeleton.h>
#include <Engine/Runtime/GameFramework/Animation/UAnimatedMeshComponent.h>
#include <Engine/Runtime/GameFramework/Animation/USkeletonComponent.h>
#include <Engine/Runtime/GameFramework/Skybox/ASkybox.h>
#include <Engine/Runtime/GameFramework/Camera/ACamera.h>
#include <Engine/Runtime/GameFramework/Pawn/APawn.h>
#include <Engine/Runtime/GameFramework/Light/UPointLightComponent.h>

namespace Engine
{
    UWorld::UWorld()
    {
        // Create Buffer
        m_FrameRenderBuffer                = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_bufferViewport = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_playground     = FrameRenderBuffer::Create();

        // GeometryBuffer
        m_GeometryBuffer = GeometryBuffer::Create();
        m_SSAOBuffer     = SSAOBuffer::Create();

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
        LoadShader("GBuffer",
                   "Assets/Editor/Shader/gbuffer_vertex.glsl",
                   "Assets/Editor/Shader/gbuffer_fragment.glsl",
                   "Path");

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

        LoadShader("Deferred",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/fragment.glsl",
                   "Path");

        LoadShader("Exposure",
                   "Assets/Editor/Shader/screen_quad_vertex.glsl",
                   "Assets/Editor/Shader/deffered/exposure.glsl",
                   "Path");

        LoadShader("Pawn", "Assets/Editor/Shader/pawn_vertex.glsl", "Assets/Editor/Shader/pawn_fragment.glsl", "Path");

        LoadShader("Trajectory",
                   "Assets/Editor/Shader/trajectory_vertex.glsl",
                   "Assets/Editor/Shader/trajectory_fragment.glsl",
                   "Path");
    }

    void UWorld::TickLogic(float timeStep, float nowTime, bool isWindowFocused)
    {
        // Render
        float m_CameraTranslationSpeed      = 7.0;
        float m_CameraTranslationSpeedMouse = 0.5;
        float m_CameraRotationSpeed         = 0.15;

        auto camrea_view     = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto pawn_view       = m_Registry.view<UTagComponent, UTransformComponent, UPawnComponent>();
        auto skeleton_view   = m_Registry.view<UTagComponent, UTransformComponent, USkeletonComponent>();
        auto trajectory_view = m_Registry.view<UTagComponent, UTransformComponent, UTrajectoryComponent>();
        auto light_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();

        auto [currentX, currentY] = Input::GetMousePostion();

        static float lastX;
        static float lastY;

        float deltaX = currentX - lastX;
        float deltaY = currentY - lastY;

        lastX = currentX;
        lastY = currentY;

        // use a range-for
        for (auto [entity, name, trans, camera] : camrea_view.each())
        {
            if (m_Registry.any_of<UPawnComponent>(entity))
            {
                continue;
            }

            AActor*  actor   = static_cast<AActor*>(camera.GetOwner());
            ACamera* aCamera = static_cast<ACamera*>(actor);

            if (isWindowFocused && actor->GetIsControlled() && aCamera->GetIsViewportCamera())
            {
                const glm::vec3 front =
                    -glm::normalize(glm::rotate(glm::quat(trans.GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f)));
                const glm::vec3 right =
                    glm::normalize(glm::rotate(glm::quat(trans.GetRotation()), glm::vec3(1.0f, 0.0f, 0.0f)));
                const glm::vec3 up(0.0f, 1.0f, 0.0f);

                if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
                {
                    // imgui lock mouse
                    // ImGui::GetIO().WantCaptureMouse = true;
                    // ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;

                    ImGui::SetMouseCursor(ImGuiMouseCursor_None);

                    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
                    {
                        auto newPosition = trans.GetPosition();
                        newPosition -= deltaY * glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) *
                                       m_CameraTranslationSpeedMouse * timeStep;
                        newPosition += deltaX * glm::normalize(glm::vec3(right[0], 0.0f, right[2])) *
                                       m_CameraTranslationSpeedMouse * timeStep;

                        trans.SetPosition(newPosition);
                    }
                    else
                    {
                        auto newRotation = trans.GetRotation();

                        newRotation.y -= deltaX * m_CameraRotationSpeed * timeStep;
                        newRotation.x -= deltaY * m_CameraRotationSpeed * timeStep;

                        trans.SetRotation(newRotation);
                    }
                }

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

        // Update Pawn

        for (auto [entity, name, trans, pawn] : pawn_view.each())
        {
            pawn.TickLogic(timeStep);
        }

        // Update Skeleton

        for (auto [entity, name, trans, skeleton] : skeleton_view.each())
        {
            skeleton.Update(fmod(0.06 * nowTime, 1.0f));
        }

        // Update Trajectory

        for (auto [entity, name, trans, trajectory] : trajectory_view.each())
        {
            trajectory.TickLogic(timeStep, cameraTransform);
        }
    }

    void UWorld::TickRender(float timeStep)
    {
        auto camrea_view     = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto model_view      = m_Registry.view<UTagComponent, UTransformComponent, UStaticMeshComponent>();
        auto pointcloud_view = m_Registry.view<UTagComponent, UTransformComponent, UPointCloudComponent>();
        auto light_view      = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();
        auto skybox_view     = m_Registry.view<UTagComponent, UTransformComponent, USkyboxComponent>();
        auto skeleton_view   = m_Registry.view<UTagComponent, UTransformComponent, USkeletonComponent>();
        auto pawn_view       = m_Registry.view<UTagComponent, UTransformComponent, UPawnComponent>();
        auto trajectory_view = m_Registry.view<UTagComponent, UTransformComponent, UTrajectoryComponent>();

        m_GeometryBuffer->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_SSAOBuffer->SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

        // Get Main SkyBox
        for (auto [entity, name, trans, skybox] : skybox_view.each())
        {
            m_MainSkybox = static_cast<ASkybox*>(skybox.GetOwner());
        }

        // Render to GeometryBuffer
        m_MainCamera->GetCamera().SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_MainCamera->GetCamera().RecalculateProjectionMatrix();

        AActor* actor_mainCamera = static_cast<AActor*>(m_MainCamera->GetOwner());

        m_VMatrix =
            glm::inverse(actor_mainCamera->GetTransformComponent().GetTransform() * m_MainCamera->GetTransform());
        m_PMatrix  = m_MainCamera->GetCamera().GetProjectionMatrix();
        m_VPMatrix = m_PMatrix * m_VMatrix;

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

            auto shader = m_ShaderLibrary.Get("GBuffer");

            shader->Bind();

            if (materialType == "BasicPbr")
            {
                const auto meshes = model.GetStaticMesh().m_Meshes;
                for (const auto& mesh : meshes)
                {
                    MBasicPbr* material_basicPbr = static_cast<MBasicPbr*>(material);
                    material_basicPbr->BindAllMap(shader);

                    shader->Bind();
                    shader->SetMat4("u_MProjection", m_PMatrix);
                    shader->SetMat4("u_MView", m_VMatrix);
                    shader->SetMat4("u_MTransform", trans.GetTransform());
                    shader->UnBind();

                    Renderer::Submit(mesh.m_VertexArray, shader, m_VPMatrix, trans.GetTransform());

                    material_basicPbr->UnBindAllMap(shader);
                }
            }

            shader->UnBind();
        }

        m_GeometryBuffer->UnBind();

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

        // Render to FrameRenderBuffer
        m_MainCamera->GetCamera().SetViewPort(m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());
        m_MainCamera->GetCamera().RecalculateProjectionMatrix();

        m_VMatrix =
            glm::inverse(actor_mainCamera->GetTransformComponent().GetTransform() * m_MainCamera->GetTransform());
        m_PMatrix  = m_MainCamera->GetCamera().GetProjectionMatrix();
        m_VPMatrix = m_PMatrix * m_VMatrix;

        m_FrameRenderBuffer->Bind();
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
                skybox.Tick(timeStep);
                skybox.Draw(shader, vpmat);
            }
        }

        // Deferred Shading
        auto deferred_shader = m_ShaderLibrary.Get("Deferred");
        deferred_shader->Bind();

        deferred_shader->SetInt("g_ViewPosition", 0);
        m_GeometryBuffer->BindViewPositionTexture(0);
        deferred_shader->SetInt("g_ViewNormal", 1);
        m_GeometryBuffer->BindViewNormalTexture(1);
        deferred_shader->SetInt("g_Albedo", 2);
        m_GeometryBuffer->BindAlbedoTexture(2);
        deferred_shader->SetInt("g_Depth", 3);
        m_GeometryBuffer->BindDepthTexture(3);
        deferred_shader->SetInt("g_Roughness", 4);
        m_GeometryBuffer->BindRoughnessTexture(4);
        deferred_shader->SetInt("g_Metallic", 5);
        m_GeometryBuffer->BindMetallicTexture(5);
        deferred_shader->SetInt("g_WorldPosition", 6);
        m_GeometryBuffer->BindWorldPositionTexture(6);
        deferred_shader->SetInt("g_WorldNormal", 7);
        m_GeometryBuffer->BindWorldNormalTexture(7);
        deferred_shader->SetInt("g_AO", 8);
        m_SSAOBuffer->BindSSAOTexture(8);

        int ligth_num = 0;
        for (auto [entity, name, trans, light] : light_view.each())
        {
            if (ligth_num >= 4)
            {
                break;
            }

            deferred_shader->SetFloat3("lightPositions[" + std::to_string(ligth_num) + "]", trans.GetPosition());
            deferred_shader->SetFloat3("lightColors[" + std::to_string(ligth_num) + "]", light.GetColorRef());

            ligth_num++;
        }
        deferred_shader->SetInt("numLights", ligth_num);

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

        // exposure
        glDepthMask(GL_FALSE);
        auto exposure_shader = m_ShaderLibrary.Get("Exposure");
        exposure_shader->Bind();

        exposure_shader->SetInt("g_Color", 0);
        m_FrameRenderBuffer->BindTexture(0);
        exposure_shader->SetFloat("exposure", m_Exposure);

        RenderCommand::RenderToQuad();

        m_FrameRenderBuffer->UnBindTexture(0);

        exposure_shader->UnBind();
        glDepthMask(GL_TRUE);

        // Point Cloud
        for (auto [entity, name, trans, pointcloud] : pointcloud_view.each())
        {
            APointCloud* pointcloud_actor = static_cast<APointCloud*>(pointcloud.GetOwner());

            if (pointcloud_actor->GetVisible() == false)
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
            ASkeleton* skeleton_actor = static_cast<ASkeleton*>(skeleton.GetOwner());

            if (skeleton_actor->GetVisible() == false)
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

        glEnable(GL_DEPTH_TEST);

        Renderer::EndScene(m_FrameRenderBuffer);

        // Render GBuffer Viewport
        m_FrameRenderBuffer_bufferViewport->Bind();
        RenderCommand::SetViewPort(
            0, 0, m_FrameRenderBuffer_bufferViewport->GetWidth(), m_FrameRenderBuffer_bufferViewport->GetHeight());

        RenderCommand::SetClearColor(m_BackGroundColor);
        RenderCommand::Clear();

        std::string gbuffer_shader_name[] = {
            "VisPosition",
            "VisNormal",
            "VisAlbedo",
            "VisDepth",
            "VisAO",
            "VisRoughness",
            "VisMetallic",
            "VisPosition",
            "VisNormal",
        };
        auto gbuffer_viewport_shader = m_ShaderLibrary.Get(gbuffer_shader_name[m_ViewportGBufferMap]);

        gbuffer_viewport_shader->Bind();

        if (m_ViewportGBufferMap == 0)
        {
            gbuffer_viewport_shader->SetInt("g_Position", 0);
            m_GeometryBuffer->BindViewPositionTexture(0);
        }
        else if (m_ViewportGBufferMap == 1)
        {
            gbuffer_viewport_shader->SetInt("g_Normal", 0);
            m_GeometryBuffer->BindViewNormalTexture(0);
        }
        else if (m_ViewportGBufferMap == 2)
        {
            gbuffer_viewport_shader->SetInt("g_Albedo", 0);
            m_GeometryBuffer->BindAlbedoTexture(0);
        }
        else if (m_ViewportGBufferMap == 3)
        {
            gbuffer_viewport_shader->SetInt("g_Depth", 0);
            m_GeometryBuffer->BindDepthTexture(0);
        }
        else if (m_ViewportGBufferMap == 4)
        {
            gbuffer_viewport_shader->SetInt("g_AO", 0);
            m_SSAOBuffer->BindSSAOTexture(0);
        }
        else if (m_ViewportGBufferMap == 5)
        {
            gbuffer_viewport_shader->SetInt("g_Roughness", 0);
            m_GeometryBuffer->BindRoughnessTexture(0);
        }
        else if (m_ViewportGBufferMap == 6)
        {
            gbuffer_viewport_shader->SetInt("g_Metallic", 0);
            m_GeometryBuffer->BindMetallicTexture(0);
        }
        else if (m_ViewportGBufferMap == 7)
        {
            gbuffer_viewport_shader->SetInt("g_Position", 0);
            m_GeometryBuffer->BindWorldPositionTexture(0);
        }
        else if (m_ViewportGBufferMap == 8)
        {
            gbuffer_viewport_shader->SetInt("g_Normal", 0);
            m_GeometryBuffer->BindWorldNormalTexture(0);
        }

        RenderCommand::RenderToQuad();

        m_GeometryBuffer->UnBindTexture(0);
        gbuffer_viewport_shader->UnBind();

        Renderer::EndScene(m_FrameRenderBuffer_bufferViewport);
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

        ImGui::Separator();

        Gui::Text("Buffer Viewport");
        const char* items[] = {
            "ViewPosition",
            "ViewNormal",
            "Albedo",
            "Depth",
            "Ambient Occlusion",
            "Roughness",
            "Metallic",
            "WorldPosition",
            "WorldNormal",
        };
        ImGui::Combo("Viewport GBuffer Map", &m_ViewportGBufferMap, items, IM_ARRAYSIZE(items));

        ImGui::Separator();

        Gui::Text("Screen Space Ambient Occlusion (SSAO)");
        ImGui::SliderFloat("Radius", &m_SSAOBuffer->GetRadiusRef(), 0.05f, 2.0f);
        ImGui::SliderFloat("Bias", &m_SSAOBuffer->GetBiasRef(), 0.0f, 0.005f);
        ImGui::SliderFloat("Power", &m_SSAOBuffer->GetPowerRef(), 0.0f, 5.0f);

        Gui::End();

        // Scence Collection
        Gui::Begin("Scence Collection");

        auto object_view      = m_Registry.view<UTagComponent, UTransformComponent>();
        auto camrea_view      = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto staticmodel_view = m_Registry.view<UTagComponent, UTransformComponent, UStaticMeshComponent>();
        auto pointcloud_view  = m_Registry.view<UTagComponent, UTransformComponent, UPointCloudComponent>();
        auto light_view       = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();
        auto skybox_view      = m_Registry.view<UTagComponent, UTransformComponent, USkyboxComponent>();
        auto skeleton_view    = m_Registry.view<UTagComponent, UTransformComponent, USkeletonComponent>();

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

                    Gui::Text("Camera Type: {}", cameraType);
                    if (ImGui::Button("Set Viewport Camera"))
                    {
                        m_MainCamera->SetIsViewportCamera(false);
                        m_MainCamera = &cameraComponent;
                        m_MainCamera->SetIsViewportCamera(true);
                    }
                    Gui::Text("Aspect: {0}", perspectiveCamera.GetAspectRatioRef());

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

                Gui::Text("Num Points: {}", pointCloudComponent.GetNumPoints());
                Gui::SliderFloat("PointRenderSize", pointCloudComponent.GetPointRenderSizeRef(), 0.0f, 100.0f);
            }

            if (m_Registry.any_of<UPointLightComponent>(entity_selected) == true)
            {
                auto& pointLightComponent = m_Registry.get<UPointLightComponent>(entity_selected);
                Gui::Text("Point Light");
                ImGui::Separator();

                ImGui::ColorEdit3("Color", glm::value_ptr(pointLightComponent.GetColorRef()));
                Gui::SliderFloat("Intensity", pointLightComponent.GetIntensityRef(), 0.0f, 1.0f);
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

                Gui::Text("Joints Num: {0}", skeletonComponent.GetNumJoints());
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