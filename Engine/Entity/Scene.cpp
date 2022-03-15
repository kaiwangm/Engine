#include "Scene.h"

#include "Entity.h"
#include "GuiCommand.h"

namespace Engine {
Scene::Scene() {  // Set FrameRenderBuffer
    m_FrameRenderBuffer = FrameRenderBuffer::Create();
    m_FrameRenderBuffer_normal = FrameRenderBuffer::Create();

    m_Camera =
        std::make_shared<PerspectiveCamera>(45.0f, 1.778f, 0.1f, 3000.0f * 8);
    m_Camera->SetPosition({0.0f, 0.0f, 3.0f});
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
    Renderer::BeginScene(m_Camera, m_FrameRenderBuffer);

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
            Renderer::Submit(mesh.m_VertexArray, shader, trans.GetTransform());
            texture->UnBind(0);
        }
    }

    Renderer::EndScene(m_FrameRenderBuffer);

    // Render
    Renderer::BeginScene(m_Camera, m_FrameRenderBuffer_normal);

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
            Renderer::Submit(mesh.m_VertexArray, shader, trans.GetTransform());
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

    auto model_view =
        m_Registry
            .view<TagComponent, TransformComponent, StaticModelComponent>();

    // use a range-for
    for (auto [entity, name, trans, model] : model_view.each()) {
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