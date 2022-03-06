#include "VertexArray.h"

#include "Renderer.h"
#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGL/OpenGLVertexArray.h"
#endif

namespace Engine {
Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            Log::Core_Error("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexArray>();
            break;

        default:
            Log::Core_Error("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}
}  // namespace Engine