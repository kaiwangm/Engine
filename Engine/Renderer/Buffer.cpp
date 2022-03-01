#include "Buffer.h"

#include "Renderer.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGL/OpenGLBuffer.h"
#endif

namespace Engine {
Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ENGINE_CORE_ERROR("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(vertices, size);
            break;

        default:
            ENGINE_CORE_ERROR("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ENGINE_CORE_ERROR("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(indices, count);
            break;

        default:
            ENGINE_CORE_ERROR("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}
}  // namespace Engine