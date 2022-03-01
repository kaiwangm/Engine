#include "Texture.h"

#include "Renderer.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGL/OpenGLTexture.h"
#endif

namespace Engine {
Ref<Texture2D> Texture2D::Create(const std::string& path) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ENGINE_CORE_ERROR("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path);
            break;

        default:
            ENGINE_CORE_ERROR("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}
}  // namespace Engine