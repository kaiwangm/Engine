#include "Renderer.h"

namespace Engine {
    #ifdef ENGINE_PLATFORM_WINDOWS
        RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;
    #else
        RendererAPI Renderer::s_RendererAPI = RendererAPI::None;
    #endif
}