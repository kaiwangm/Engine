#pragma once

#include "Core.h"
#include "RendererAPI.h"

namespace Engine {

class RenderCommand {
   public:
    inline static void Init()
    {
        s_RendererAPI->Init();
    }
    static void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }

    static void Clear() { s_RendererAPI->Clear(); }

    static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
        s_RendererAPI->DrawIndexed(vertexArray);
    }

   private:
    static Scope<RendererAPI> s_RendererAPI;
};

}  // namespace Engine