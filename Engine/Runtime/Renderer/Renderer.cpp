#include <Engine/Runtime/Renderer/Renderer.h>
#include <Engine/Runtime/GameFramework/Camera/Camera.h>

namespace Engine
{
    Scope<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

    void Renderer::Init() { RenderCommand::Init(); }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height) { RenderCommand::SetViewPort(0, 0, width, height); }

    void Renderer::BeginScene(const Ref<Camera>& camera)
    {
        // s_SceneData->ViewProjectionMatrix = camera->GetViewProjectMatrix();
    }

    void Renderer::BeginScene(const Ref<Camera>& camera, const Ref<FrameRenderBuffer> framerenderbuffer)
    {
        camera->SetViewPort(framerenderbuffer->GetWidth(), framerenderbuffer->GetHeight());
        camera->RecalculateProjectionMatrix();

        // s_SceneData->ViewProjectionMatrix = camera->GetViewProjectMatrix();

        framerenderbuffer->Bind();

        RenderCommand::SetViewPort(0, 0, framerenderbuffer->GetWidth(), framerenderbuffer->GetHeight());
    }

    void Renderer::EndScene() {}

    void Renderer::EndScene(const Ref<FrameRenderBuffer> framerenderbuffer) { framerenderbuffer->UnBind(); }

    void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);

        vertexArray->UnBind();
        shader->UnBind();
    }

    void Renderer::Submit(const Ref<VertexArray>& vertexArray,
                          const Ref<Shader>&      shader,
                          const glm::mat4&        pvMatrix,
                          const glm::mat4&        transform)
    {
        // shader->Bind();
        // shader->SetMat4("u_ViewProjection", pvMatrix);
        // shader->SetMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);

        vertexArray->UnBind();
        // shader->UnBind();
    }

    void Renderer::DrawArray(const Ref<VertexArray>& vertexArray,
                             const Ref<Shader>&      shader,
                             const glm::mat4&        pvMatrix,
                             const glm::mat4&        transform)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", pvMatrix);
        shader->SetMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawArray(vertexArray);

        vertexArray->UnBind();
        shader->UnBind();
    }

    void Renderer::DrawSkybox(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& pvMatrix)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", pvMatrix);

        vertexArray->Bind();
        RenderCommand::DrawSkybox(vertexArray);

        vertexArray->UnBind();
        shader->UnBind();
    }

} // namespace Engine