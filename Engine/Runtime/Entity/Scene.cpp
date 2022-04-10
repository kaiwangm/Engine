#include "Scene.h"

#include "Entity.h"
#include "GuiCommand.h"
#include "Input/Input.h"

namespace Engine {
Scene::Scene() {  // Set FrameRenderBuffer
    m_FrameRenderBuffer = FrameRenderBuffer::Create();
    m_FrameRenderBuffer_normal = FrameRenderBuffer::Create();
}

Scene::~Scene() {}

Entity Scene::CreateEntity(const Ref<Scene> scene, const std::string& name) {
    Entity entity(scene->m_Registry.create(), scene);
    // entity.AddComponent<UTransformComponent>();
    auto& tag = entity.AddComponent<UTagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;
    return entity;
}

void Scene::DestroyEntity(const Ref<Scene> scene, Entity entity) {
    scene->m_Registry.destroy(entity);
}

void Scene::OnUpdateRuntime(float timeStep) {
    auto camrea_view =
        m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();
    glm::mat4 vp_Mat;

    // use a range-for
    for (auto [entity, name, trans, camera] : camrea_view.each()) {
        auto camera_ref = camera.GetCamera();

        camera_ref->SetViewPort(m_FrameRenderBuffer->GetWidth(),
                                m_FrameRenderBuffer->GetHeight());
        camera_ref->RecalculateViewProjectMatrix();

        vp_Mat = camera_ref->GetProjectionMatrix() *
                 glm::inverse(trans.GetTransform());
    }

    m_FrameRenderBuffer->Bind();
    RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer->GetWidth(),
                               m_FrameRenderBuffer->GetHeight());

    RenderCommand::SetClearColor(backGroundColor);
    RenderCommand::Clear();

    auto model_view =
        m_Registry
            .view<UTagComponent, UTransformComponent, UStaticModelComponent>();

    // use a range-for
    for (auto [entity, name, trans, model] : model_view.each()) {
        auto shader = m_ShaderLibrary.Get("TextureShader");
        Renderer::SetShaderUniform(shader, "u_Texture", 0);

        const auto& meshes = model.GetModel()->m_Meshes;

        for (const auto& mesh : meshes) {
            auto texture = mesh.m_Textures[0];
            texture->Bind(0);
            Renderer::Submit(mesh.m_VertexArray, shader, vp_Mat,
                             trans.GetTransform());
            texture->UnBind(0);
        }
    }

    auto animated_view =
        m_Registry
            .view<UTagComponent, UTransformComponent, UAnimatedModelComponent>();

    // use a range-for
    for (auto [entity, name, trans, model] : animated_view.each()) {
        auto shader = m_ShaderLibrary.Get("Animated");
        // Renderer::SetShaderUniform(shader, "u_Texture", 0);

        model.GetModel()->Update(timeStep);
        model.GetModel()->Draw(shader, vp_Mat, trans.GetTransform());
    }

    Renderer::EndScene(m_FrameRenderBuffer);

    // Render

    // use a range-for
    for (auto [entity, name, trans, camera] : camrea_view.each()) {
        auto camera_ref = camera.GetCamera();

        camera_ref->SetViewPort(m_FrameRenderBuffer_normal->GetWidth(),
                                m_FrameRenderBuffer_normal->GetHeight());
        camera_ref->RecalculateViewProjectMatrix();

        vp_Mat = camera_ref->GetProjectionMatrix() *
                 glm::inverse(trans.GetTransform());
    }

    m_FrameRenderBuffer_normal->Bind();
    RenderCommand::SetViewPort(0, 0, m_FrameRenderBuffer_normal->GetWidth(),
                               m_FrameRenderBuffer_normal->GetHeight());

    RenderCommand::SetClearColor(backGroundColor);
    RenderCommand::Clear();

    // use a range-for
    for (auto [entity, name, trans, model] : model_view.each()) {
        auto shader = m_ShaderLibrary.Get("TextureShader_normal");
        Renderer::SetShaderUniform(shader, "u_Texture", 0);

        const auto& meshes = model.GetModel()->m_Meshes;

        for (const auto& mesh : meshes) {
            auto texture = mesh.m_Textures[0];
            texture->Bind(0);
            Renderer::Submit(mesh.m_VertexArray, shader, vp_Mat,
                             trans.GetTransform());
            texture->UnBind(0);
        }
    }

    Renderer::EndScene(m_FrameRenderBuffer_normal);
}

void Scene::OnUpdateRuntimeGui(float timeStep, float nowTime) {
    Gui::Begin("Scence Collection");

    Gui::Text("Time Step: {0}", timeStep);
    Gui::Text("Now Time: {0}", nowTime);

    static std::vector<float> arr(600, 0.0);
    arr.push_back(timeStep);
    arr.erase(arr.begin());
    ImGui::PlotLines("Frame Times", &arr[0], 600);

    auto model_view = m_Registry.view<UTagComponent, UTransformComponent>();

    // use a range-for
    for (auto [entity, name, trans] : model_view.each()) {
        if (ImGui::TreeNode(name.GetString().c_str())) {
            Gui::DragFloat3("Position", trans.Translation, 0.005f, -100.0f,
                            100.0f);
            Gui::DragFloat3("Rotation", trans.Rotation, 0.005f, -100.0f,
                            100.0f);
            Gui::DragFloat3("Scale", trans.Scale, 0.005f, -100.0f, 100.0f);

            ImGui::TreePop();
        }
    }

    Gui::End();
}

void Scene::TickLogic(float timeStep, float nowTime, bool handleInput) {
    // Render
    float m_CameraTranslationSpeed = 7.0;
    float m_CameraTranslationSpeedMouse = 0.5;
    float m_CameraRotationSpeed = 0.15;

    auto camrea_view =
        m_Registry.view<UTagComponent, UTransformComponent, UCameraComponent>();

    auto [currentX, currentY] = Input::GetMousePostion();

    static float lastX;
    static float lastY;

    float deltaX = currentX - lastX;
    float deltaY = currentY - lastY;

    // use a range-for
    for (auto [entity, name, trans, camera] : camrea_view.each()) {
        if (camera.GetCamera()->m_IsWindowFocused) {
            if (handleInput) {
                const glm::vec3 front = -glm::normalize(glm::rotate(
                    glm::quat(trans.Rotation), glm::vec3(0.0f, 0.0f, 1.0f)));
                const glm::vec3 right = glm::normalize(glm::rotate(
                    glm::quat(trans.Rotation), glm::vec3(1.0f, 0.0f, 0.0f)));
                const glm::vec3 up(0.0f, 1.0f, 0.0f);

                if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2)) {
                    // io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
                    ImGui::SetMouseCursor(ImGuiMouseCursor_None);

                    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
                        trans.Translation -=
                            deltaY *
                            glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) *
                            m_CameraTranslationSpeedMouse * timeStep;
                        trans.Translation += deltaX *
                                             glm::normalize(glm::vec3(
                                                 right[0], 0.0f, right[2])) *
                                             m_CameraTranslationSpeedMouse *
                                             timeStep;
                    } else {
                        trans.Rotation.y -=
                            deltaX * m_CameraRotationSpeed * timeStep;
                        trans.Rotation.x -=
                            deltaY * m_CameraRotationSpeed * timeStep;
                    }
                }

                if (Input::IsKeyPressed(GLFW_KEY_A)) {
                    trans.Translation -=
                        right * m_CameraTranslationSpeed * timeStep;
                }
                if (Input::IsKeyPressed(GLFW_KEY_D)) {
                    trans.Translation +=
                        right * m_CameraTranslationSpeed * timeStep;
                }
                if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
                    trans.Translation -=
                        up * m_CameraTranslationSpeed * timeStep;
                }
                if (Input::IsKeyPressed(GLFW_KEY_SPACE)) {
                    trans.Translation +=
                        up * m_CameraTranslationSpeed * timeStep;
                }
                if (Input::IsKeyPressed(GLFW_KEY_W)) {
                    trans.Translation +=
                        front * m_CameraTranslationSpeed * timeStep;
                }
                if (Input::IsKeyPressed(GLFW_KEY_S)) {
                    trans.Translation -=
                        front * m_CameraTranslationSpeed * timeStep;
                }
            }
        }
    }

    lastX = currentX;
    lastY = currentY;
}

void Scene::TickRender(float timeStep, float nowTime) {
    OnUpdateRuntime(timeStep);
    OnUpdateRuntimeGui(timeStep, nowTime);
}

void Scene::LoadShader(const std::string& name, const std::string& vertexSrc,
                       const std::string& fragmentSrc,
                       const std::string& mode) {
    m_ShaderLibrary.Load(name, vertexSrc, fragmentSrc, mode);
}
}  // namespace Engine