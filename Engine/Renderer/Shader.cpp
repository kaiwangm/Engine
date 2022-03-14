#include "Shader.h"

#include "Log.h"
#include "Renderer.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGL_Renderer/OpenGLShader.h"
#endif

namespace Engine {
Ref<Shader> Shader::Create(const std::string& name,
                           const std::string& vertexSrc,
                           const std::string& fragmentSrc,
                           const std::string& mode) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            Log::Core_Error("RendererAPI::None is currently not supported.");
            break;

        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc,
                                                  mode);
            break;

        default:
            Log::Core_Error("RendererAPI::Unknow API.");
            break;
    }

    return nullptr;
}

void ShaderLibrary::Add(const Ref<Shader>& shader) {
    auto& name = shader->GetName();
    m_Shaders[name] = shader;
}

const Ref<Shader>& ShaderLibrary::Load(const std::string& name,
                                       const std::string& vertexSrc,
                                       const std::string& fragmentSrc,
                                       const std::string& mode) {
    auto shader = Shader::Create(name, vertexSrc, fragmentSrc, mode);
    m_Shaders[name] = shader;

    return m_Shaders[name];
}

const Ref<Shader>& ShaderLibrary::Get(const std::string& name) {
    return m_Shaders[name];
}
}  // namespace Engine