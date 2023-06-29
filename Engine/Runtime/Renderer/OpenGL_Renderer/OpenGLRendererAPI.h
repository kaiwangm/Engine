#pragma once
#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Renderer/RendererAPI.h>

namespace Engine
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(const glm::vec4& color) override
        {
            glClearColor(color.r, color.g, color.b, color.a);
        }
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
        virtual void DrawArray(const Ref<VertexArray>& vertexArray) override;
        virtual void DrawSkybox(const Ref<VertexArray>& vertexArray) override;

        virtual void RenderToQuad() override;

    private:
        static GLuint quadVAO;
        static GLuint quadVBO;
    };
} // namespace Engine