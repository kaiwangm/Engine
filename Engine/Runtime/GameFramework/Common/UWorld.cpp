#include "UWorld.h"

#include <memory>

#include "Shader.h"
#include "Buffer.h"
#include "GuiCommand.h"
#include "Input/Input.h"

#include "UComponent.h"
#include "../Camera/UCameraComponent.h"
#include "../StaticMesh/AStaticMesh.h"
#include "../Animation/UAnimatedMeshComponent.h"
#include "../Skybox/ASkybox.h"
#include "../Camera/ACamera.h"
#include "../Light/UPointLightComponent.h"

namespace Engine
{
    UWorld::UWorld()
    {
        // Create Buffer
        m_FrameRenderBuffer        = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_normal = FrameRenderBuffer::Create();
    }

    void UWorld::TickLogic(float timeStep, float nowTime)
    {
        // Render
        float m_CameraTranslationSpeed      = 7.0;
        float m_CameraTranslationSpeedMouse = 0.5;
        float m_CameraRotationSpeed         = 0.15;

        auto camrea_view = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();

        auto [currentX, currentY] = Input::GetMousePostion();

        static float lastX;
        static float lastY;

        float deltaX = currentX - lastX;
        float deltaY = currentY - lastY;

        // use a range-for
        for (auto [entity, name, trans, camera] : camrea_view.each())
        {
            // Log::Info("{0}", camera.GetCamera().m_IsWindowFocused);
            if (camera.GetCamera().m_IsWindowFocused)
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

        lastX = currentX;
        lastY = currentY;
    }

    void UWorld::TickRender(float timeStep)
    {

        auto camrea_view   = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto model_view    = m_Registry.view<UTagComponent, UTransformComponent, UStaticMeshComponent>();
        auto animated_view = m_Registry.view<UTagComponent, UTransformComponent, UAnimatedMeshComponent>();
        auto light_view    = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();
        auto skybox_view   = m_Registry.view<UTagComponent, UTransformComponent, USkyboxComponent>();

        // Get Main Camera
        for (auto [entity, name, trans, camera] : camrea_view.each())
        {
            m_MainCamera = static_cast<ACamera*>(camera.GetOwner());
        }

        // Get Main SkyBox
        for (auto [entity, name, trans, skybox] : skybox_view.each())
        {
            m_MainSkybox = static_cast<ASkybox*>(skybox.GetOwner());
        }

        // Render to FrameRenderBuffer
        m_MainCamera->GetCameraComponent().GetCamera().SetViewPort(m_FrameRenderBuffer->GetWidth(),
                                                                   m_FrameRenderBuffer->GetHeight());
        m_MainCamera->GetCameraComponent().GetCamera().RecalculateViewProjectMatrix();

        m_VMatrix  = glm::inverse(m_MainCamera->GetTransformComponent().GetTransform());
        m_PMatrix  = m_MainCamera->GetCameraComponent().GetCamera().GetViewProjectMatrix();
        m_VPMatrix = m_PMatrix * m_VMatrix;

        m_FrameRenderBuffer->Bind();
        RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(), m_FrameRenderBuffer->GetHeight());

        RenderCommand::SetClearColor(m_BackGroundColor);
        RenderCommand::Clear();

        // use a range-for
        for (auto [entity, name, trans, model] : model_view.each())
        {
            AStaticMesh* staticMesh_actor = static_cast<AStaticMesh*>(model.GetOwner());
            MBasicPbr*   material         = static_cast<MBasicPbr*>(staticMesh_actor->GetMaterial());

            auto shader = m_ShaderLibrary.Get("BasicPbr");
            shader->Bind();

            int ligth_num = 0;
            // use a range-for
            for (auto [entity, name, trans, light] : light_view.each())
            {
                if (ligth_num >= 4)
                {
                    break;
                }

                Renderer::SetShaderUniform(
                    shader, "lightPositions[" + std::to_string(ligth_num) + "]", trans.GetPosition());
                Renderer::SetShaderUniform(
                    shader, "lightColors[" + std::to_string(ligth_num) + "]", light.GetColorRef());

                ligth_num++;
            }

            Renderer::SetShaderUniform(shader, "camPos", m_MainCamera->GetTransformComponent().GetPosition());

            const auto meshes = model.GetStaticMesh().m_Meshes;

            for (const auto& mesh : meshes)
            {
                material->Bind(shader);

                auto sky_cubeMap = m_MainSkybox->GetSkyboxComponent().GetCubeMap();

                sky_cubeMap->BindIrradianceTexture(5);
                sky_cubeMap->BindPrefilterTexture(6);
                sky_cubeMap->BindBrdfLutTexture(7);

                Renderer::Submit(mesh.m_VertexArray, shader, m_VPMatrix, trans.GetTransform());

                sky_cubeMap->UnBind(7);
                sky_cubeMap->UnBind(6);
                sky_cubeMap->UnBind(5);

                material->UnBind(shader);
            }

            shader->UnBind();
        }

        // use a range-for
        for (auto [entity, name, trans, model] : animated_view.each())
        {
            auto shader = m_ShaderLibrary.Get("Animated");
            // Renderer::SetShaderUniform(shader, "u_Texture", 0);

            model.GetModel().Update(timeStep);
            model.GetModel().Draw(shader, m_VPMatrix, trans.GetTransform());
        }

        // draw skybox
        // use a range-for
        for (auto [entity, name, trans, skybox] : skybox_view.each())
        {
            auto shader = m_ShaderLibrary.Get("Skybox");
            Renderer::SetShaderUniform(shader, "exposure", m_Exposure);
            Renderer::SetShaderUniform(shader, "mipLevel", m_VisPrePrefilterMipLevel);

            auto vpmat = m_PMatrix * glm::mat4(glm::mat3(m_VMatrix));
            skybox.Tick(timeStep);
            skybox.Draw(shader, vpmat);
        }

        Renderer::EndScene(m_FrameRenderBuffer);

        // Render
        m_MainCamera->GetCameraComponent().GetCamera().SetViewPort(m_FrameRenderBuffer_normal->GetWidth(),
                                                                   m_FrameRenderBuffer_normal->GetHeight());
        m_MainCamera->GetCameraComponent().GetCamera().RecalculateViewProjectMatrix();
        m_VPMatrix = m_MainCamera->GetCameraComponent().GetCamera().GetViewProjectMatrix() *
                     glm::inverse(m_MainCamera->GetTransformComponent().GetTransform());

        m_FrameRenderBuffer_normal->Bind();
        RenderCommand::SetViewPort(
            0, 0, m_FrameRenderBuffer_normal->GetWidth(), m_FrameRenderBuffer_normal->GetHeight());

        RenderCommand::SetClearColor(m_BackGroundColor);
        RenderCommand::Clear();

        // use a range-for
        for (auto [entity, name, trans, model] : model_view.each())
        {
            auto shader = m_ShaderLibrary.Get("TextureShader_normal");
            Renderer::SetShaderUniform(shader, "u_Texture", 0);

            const auto meshes = model.GetStaticMesh().m_Meshes;

            for (const auto& mesh : meshes)
            {
                auto texture = mesh.m_Textures[0];
                texture->Bind(0);
                Renderer::Submit(mesh.m_VertexArray, shader, m_VPMatrix, trans.GetTransform());
                texture->UnBind(0);
            }
        }

        Renderer::EndScene(m_FrameRenderBuffer_normal);
    }

    void UWorld::TickGui(float timeStep)
    {
        // Render Setting
        Gui::Begin("Render Setting");

        Gui::SliderFloat("Exposure", m_Exposure, 0.0f, 3.0f);

        Gui::End();

        // Scence Collection
        Gui::Begin("Scence Collection");

        auto camrea_view      = m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
        auto staticmodel_view = m_Registry.view<UTagComponent, UTransformComponent, UStaticMeshComponent>();
        auto animated_view    = m_Registry.view<UTagComponent, UTransformComponent, UAnimatedMeshComponent>();
        auto light_view       = m_Registry.view<UTagComponent, UTransformComponent, UPointLightComponent>();
        auto skybox_view      = m_Registry.view<UTagComponent, UTransformComponent, USkyboxComponent>();

        // Static Model
        for (auto [entity, name, trans, model] : staticmodel_view.each())
        {
            if (ImGui::TreeNode(name.GetString().c_str()))
            {
                AStaticMesh* staticMesh_actor = static_cast<AStaticMesh*>(model.GetOwner());
                MBasicPbr*   material         = static_cast<MBasicPbr*>(staticMesh_actor->GetMaterial());

                // Actor
                Gui::DragFloat3("Position", trans.GetPositionRef(), 0.005f, -100.0f, 100.0f);
                Gui::DragFloat3("Rotation", trans.GetRotationRef(), 0.005f, -100.0f, 100.0f);
                Gui::DragFloat3("Scale", trans.GetScaleRef(), 0.005f, -100.0f, 100.0f);

                // Line
                ImGui::Separator();

                // Material
                ImGui::ColorEdit3("Albedo", glm::value_ptr(material->GetAlbedoRef()));
                Gui::SliderFloat("Metallic", material->GetMetallicRef(), 0.0f, 1.0f);
                Gui::SliderFloat("Roughness", material->GetRoughnessRef(), 0.0f, 1.0f);
                Gui::SliderFloat("AO", material->GetAORef(), 0.0f, 1.0f);

                ImGui::TreePop();
            }
        }

        // Light
        for (auto [entity, name, trans, light] : light_view.each())
        {
            if (ImGui::TreeNode(name.GetString().c_str()))
            {
                // Actor
                Gui::DragFloat3("Position", trans.GetPositionRef(), 0.005f, -100.0f, 100.0f);

                // Line
                ImGui::Separator();

                // Light
                ImGui::ColorEdit3("Color", glm::value_ptr(light.GetColorRef()));
                Gui::SliderFloat("Intensity", light.GetIntensityRef(), 0.0f, 1.0f);

                ImGui::TreePop();
            }
        }

        // SkyBox
        for (auto [entity, name, trans, sky] : skybox_view.each())
        {
            if (ImGui::TreeNode(name.GetString().c_str()))
            {
                // Actor
                Gui::DragFloat3("Rotation", trans.GetRotationRef(), 0.005f, -100.0f, 100.0f);

                // Line
                ImGui::Separator();

                // Skybox
                // use imgui add a bool selector

                const char* items[] = {"CubeMap", "Irradiance", "Prefilter"};
                ImGui::Combo("Show Map", &sky.GetCruuentTextureIndexRef(), items, IM_ARRAYSIZE(items));

                Gui::SliderFloat("Mipmap Level", m_VisPrePrefilterMipLevel, 0.0f, 32.0f);

                ImGui::TreePop();
            }
        }

        // Camera
        for (auto [entity, name, trans, camera] : camrea_view.each())
        {
            if (ImGui::TreeNode(name.GetString().c_str()))
            {
                // Actor
                Gui::DragFloat3("Position", trans.GetPositionRef(), 0.005f, -100.0f, 100.0f);
                Gui::DragFloat3("Rotation", trans.GetRotationRef(), 0.005f, -100.0f, 100.0f);
                Gui::DragFloat3("Scale", trans.GetScaleRef(), 0.005f, -100.0f, 100.0f);

                ImGui::TreePop();
            }
        }

        Gui::End();
    }
}; // namespace Engine