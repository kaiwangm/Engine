#pragma once

#include "Core.h"
#include "RendererAPI.h"

namespace Engine
{

    class RenderCommand
    {
    public:
        inline static void Init() { s_RendererAPI->Init(); }

        inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewPort(x, y, width, height);
        }

        static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }

        static void Clear() { s_RendererAPI->Clear(); }

        static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }

        static void DrawArray(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawArray(vertexArray); }

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };

} // namespace Engine