#include "Shader.h"

#include "Log.h"
#include "Renderer.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGL/OpenGLShader.h"
#endif

namespace Engine {
Ref<Shader> Shader::Create(const std::string& vertexSrc,
                           const std::string& fragmentSrc,
                           const std::string& mode) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ENGINE_CORE_ERROR("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc, mode);
            break;

        default:
            ENGINE_CORE_ERROR("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}
}  // namespace Engine