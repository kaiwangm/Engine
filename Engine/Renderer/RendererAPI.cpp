#include "RendererAPI.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGL/OpenGLRendererAPI.h"
#endif

namespace Engine {
#ifdef ENGINE_PLATFORM_WINDOWS
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#else
RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;
#endif

Scope<RendererAPI> RendererAPI::Create() {
    switch (s_API) {
        case RendererAPI::API::None:
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_unique<OpenGLRendererAPI>();
    }

    return nullptr;
}
}  // namespace Engine