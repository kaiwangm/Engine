#include "Renderer.h"

namespace Engine {
Scope<Renderer::SceneData> Renderer::s_SceneData =
    std::make_unique<Renderer::SceneData>();

void Renderer::Init() { RenderCommand::Init(); }

void Renderer::BeginScene(const Ref<Camera>& camera) {
    s_SceneData->ViewProjectionMatrix = camera->GetViewProjectMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<VertexArray>& vertexArray,
                      const Ref<Shader>& shader, const glm::mat4& transform) {
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->SetMat4("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);

    vertexArray->UnBind();
    shader->UnBind();
}

void Renderer::DrawArray(const Ref<VertexArray>& vertexArray,
                      const Ref<Shader>& shader, const glm::mat4& transform) {
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->SetMat4("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawArray(vertexArray);

    vertexArray->UnBind();
    shader->UnBind();
}

}  // namespace Engine