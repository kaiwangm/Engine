#include <Engine/Runtime/Renderer/OpenGL_Renderer/OpenGLRendererAPI.h>

namespace Engine
{
    GLuint OpenGLRendererAPI::quadVAO = 0;
    GLuint OpenGLRendererAPI::quadVBO = 0;

    void OpenGLRendererAPI::Init()
    {
        // glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        // glDrawArraysInstanced(GL_TRIANGLES,
        //                       0,
        //                       vertexArray->GetVertexBuffers()[0]->GetCount(),
        //                       vertexArray->GetVertexBuffers()[1]->GetCount());
    }

    void OpenGLRendererAPI::DrawSkybox(const Ref<VertexArray>& vertexArray)
    {
        // glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        glDepthFunc(GL_LESS);
        // glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::RenderToQuad()
    {
        if (quadVAO == 0)
        {
            GLfloat quadVertices[] = {
                // Positions        // Texture Coords
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // Setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

} // namespace Engine