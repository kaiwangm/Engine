#include "OpenGLRendererAPI.h"

namespace Engine
{
    void OpenGLRendererAPI::Init()
    {
        // glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }
    void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawArray(const Ref<VertexArray>& vertexArray)
    {
        // glDrawArrays(GL_TRIANGLES, 0,
        // vertexArray->GetVertexBuffers()[0]->GetCount());
        glDrawArraysInstanced(GL_TRIANGLES,
                              0,
                              vertexArray->GetVertexBuffers()[0]->GetCount(),
                              vertexArray->GetVertexBuffers()[1]->GetCount());
    }

    void OpenGLRendererAPI::DrawSkybox(const Ref<VertexArray>& vertexArray)
    {
        // glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        glDepthFunc(GL_LESS);
        // glEnable(GL_DEPTH_TEST);
    }

} // namespace Engine