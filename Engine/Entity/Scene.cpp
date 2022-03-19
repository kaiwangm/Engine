#include "Scene.h"

#include "Entity.h"
#include "GuiCommand.h"
#include "Input.h"

namespace Engine {
Scene::Scene() {  // Set FrameRenderBuffer
    m_FrameRenderBuffer = FrameRenderBuffer::Create();
    m_FrameRenderBuffer_normal = FrameRenderBuffer::Create();
}

Scene::~Scene() {}

Entity Scene::CreateEntity(const Ref<Scene> scene, const std::string& name) {
    Entity entity(scene->m_Registry.create(), scene);
    // entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;
    return entity;
}

void Scene::DestroyEntity(const Ref<Scene> scene, Entity entity) {
    scene->m_Registry.destroy(entity);
}

void Scene::OnUpdateRuntime(float timeStep) {
    // Render
    float m_CameraTranslationSpeed = 3.0;
    float m_CameraRotationSpeed = 3.0;

    auto camrea_view =
        m_Registry.view<TagComponent, TransformComponent, CameraComponent>();
    glm::mat4 vp_Mat;

    // use a range-for
    for (auto [entity, name, trans, camera] : camrea_view.each()) {
        if (camera.GetCamera()->m_IsWindowFocused) {
            if (Input::IsKeyPressed(GLFW_KEY_A)) {
                trans.Translation.x -= m_CameraTranslationSpeed * timeStep;
            }
            if (Input::IsKeyPressed(GLFW_KEY_D)) {
                trans.Translation.x += m_CameraTranslationSpeed * timeStep;
            }
            if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
                trans.Translation.y -= m_CameraTranslationSpeed * timeStep;
            }
            if (Input::IsKeyPressed(GLFW_KEY_SPACE)) {
                trans.Translation.y += m_CameraTranslationSpeed * timeStep;
            }
            if (Input::IsKeyPressed(GLFW_KEY_W)) {
                trans.Translation.z -= m_CameraTranslationSpeed * timeStep;
            }
            if (Input::IsKeyPressed(GLFW_KEY_S)) {
                trans.Translation.z += m_CameraTranslationSpeed * timeStep;
            }
            if (Input::IsKeyPressed(GLFW_KEY_E)) {
                trans.Rotation.y -= m_CameraRotationSpeed * timeStep;
            }
            if (Input::IsKeyPressed(GLFW_KEY_Q)) {
                trans.Rotation.y += m_CameraRotationSpeed * timeStep;
            }
        }

        auto& camera_ref = camera.GetCamera();

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
            .view<TagComponent, TransformComponent, StaticModelComponent>();

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

    Renderer::EndScene(m_FrameRenderBuffer);

    // Render

    // use a range-for
    for (auto [entity, name, trans, camera] : camrea_view.each()) {
        auto& camera_ref = camera.GetCamera();

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

    auto model_view = m_Registry.view<TagComponent, TransformComponent>();

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

void Scene::LoadShader(const std::string& name, const std::string& vertexSrc,
                       const std::string& fragmentSrc,
                       const std::string& mode) {
    m_ShaderLibrary.Load(name, vertexSrc, fragmentSrc, mode);
}
}  // namespace Engine