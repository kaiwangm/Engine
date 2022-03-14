#include "Texture.h"

#include "Renderer.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGL_Renderer/OpenGLTexture.h"
#endif

namespace Engine {
Ref<Texture2D> Texture2D::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            Log::Core_Error("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>();
            break;

        default:
            Log::Core_Error("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            Log::Core_Error("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path);
            break;

        default:
            Log::Core_Error("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}
}  // namespace Engine