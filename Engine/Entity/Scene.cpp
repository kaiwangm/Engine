#include "Scene.h"

#include "Entity.h"
#include "GuiCommand.h"

namespace Engine {
Scene::Scene() {}

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
    auto view =
        m_Registry
            .view<TagComponent, TransformComponent, RenderableMeshComponent>();

    // use a range-for
    for (auto [entity, name, trans, renderableMesh] : view.each()) {
        auto shader = renderableMesh.GetShader();
        Renderer::SetShaderUniform(shader, "u_Color",
                                   glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
        Renderer::SetShaderUniform(shader, "u_Texture", 0);

        auto texture = renderableMesh.GetTexture();
        texture->Bind(0);
        Renderer::Submit(renderableMesh.GetVertexArray(), shader,
                         trans.GetTransform());
        texture->UnBind(0);
    }
}

void Scene::OnUpdateRuntimeGui(float timeStep, float nowTime) {
    Gui::Begin("Scence Collection");

    Gui::Text("Hello, a Tree and a Square !");
    Gui::Text("Time Step: {0}", timeStep);
    Gui::Text("Now Time: {0}", nowTime);

    static std::vector<float> arr(600, 0.0);
    arr.push_back(timeStep);
    arr.erase(arr.begin());
    ImGui::PlotLines("Frame Times", &arr[0], 600);

    auto view =
        m_Registry
            .view<TagComponent, TransformComponent, RenderableMeshComponent>();

    // use a range-for
    for (auto [entity, name, trans, renderableMesh] : view.each()) {
        if (ImGui::TreeNode(name.GetString().c_str())) {
            Gui::DragFloat3("Position", trans.Translation, 0.005f, -100.0f, 100.0f);
            Gui::DragFloat3("Rotation", trans.Rotation, 0.005f, -100.0f, 100.0f);
            Gui::DragFloat3("Scale", trans.Scale, 0.005f, -100.0f, 100.0f);

            ImGui::TreePop();
        }
    }

    Gui::End();
}
}  // namespace Engine