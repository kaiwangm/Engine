#include "Buffer.h"

#include "Renderer.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGL/OpenGLBuffer.h"
#endif

namespace Engine {
Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size,
                                       uint32_t count) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            Log::Core_Error("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(vertices, size, count);
            break;

        default:
            Log::Core_Error("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            Log::Core_Error("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(indices, count);
            break;

        default:
            Log::Core_Error("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}

Ref<FrameRenderBuffer> FrameRenderBuffer::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            Log::Core_Error("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLFrameRenderBuffer>();
            break;

        default:
            Log::Core_Error("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}
}  // namespace Engine