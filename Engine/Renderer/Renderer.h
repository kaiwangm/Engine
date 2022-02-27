#pragma once

#include "Core.h"
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Engine {
class Renderer {
   public:
    static void BeginScene();
    static void EndScene();

    static void Submit(const Ref<VertexArray>& vertexArray,
                       const Ref<Shader>& shader);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};
}  // namespace Engine