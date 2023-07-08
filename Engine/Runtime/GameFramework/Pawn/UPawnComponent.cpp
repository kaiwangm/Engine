#include <Engine/Runtime/GameFramework/Pawn/UPawnComponent.h>

namespace Engine
{
    UPawnComponent::UPawnComponent() : m_PawnStaticMesh("Assert/Editor/Object/apawn/apawn.obj") {}

    UPawnComponent::~UPawnComponent() {}

    void UPawnComponent::Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform)
    {
        shader->Bind();

        shader->SetMat4("u_ViewProjection", vpMat);
        shader->SetMat4("u_Transform", transform);
        shader->SetFloat3("u_Color", glm::vec3(0.7f, 0.0f, 0.0f));

        auto& vertexArray = m_PawnStaticMesh.GetStaticMesh().m_Meshes[0].m_VertexArray;
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);

        vertexArray->UnBind();

        shader->UnBind();
    }

} // namespace Engine