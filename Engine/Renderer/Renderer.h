#pragma once

#include "Camera.h"
#include "Core.h"
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Engine {
class Renderer {
   public:
    static void BeginScene(const Ref<Camera>& camera);
    static void EndScene();

    static void Submit(const Ref<VertexArray>& vertexArray,
                       const Ref<Shader>& shader);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

   private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static Scope<SceneData> s_SceneData;
};
}  // namespace Engine