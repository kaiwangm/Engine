#include "Renderer.h"

namespace Engine {
Scope<Renderer::SceneData> Renderer::s_SceneData =
    std::make_unique<Renderer::SceneData>();

void Renderer::BeginScene(const Ref<Camera>& camera) {
    s_SceneData->ViewProjectionMatrix = camera->GetViewProjectMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<VertexArray>& vertexArray,
                      const Ref<Shader>& shader) {
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);


    vertexArray->UnBind();
    shader->UnBind();
}
}  // namespace Engine