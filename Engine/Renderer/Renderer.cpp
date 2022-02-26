#include "Renderer.h"

namespace Engine {
    void Renderer::BeginScene()
	{
		
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader)
	{
		shader->Bind();
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->UnBind();
		shader->UnBind();
	}
}