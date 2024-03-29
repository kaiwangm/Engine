#include <Engine/Runtime/Renderer/Shader.h>
#include <Engine/Runtime/Renderer/Renderer.h>

#ifdef ENGINE_PLATFORM_OPENGL
#include <Engine/Runtime/Renderer/OpenGL_Renderer/OpenGLShader.h>
#endif

namespace Engine
{
    Ref<Shader> Shader::Create(const std::string& name,
                               const std::string& vertexSrc,
                               const std::string& fragmentSrc,
                               const std::string& mode)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc, mode);
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name,
                               const std::string& vertexSrc,
                               const std::string& geometrySrc,
                               const std::string& fragmentSrc,
                               const std::string& mode)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name, vertexSrc, geometrySrc, fragmentSrc, mode);
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name      = shader->GetName();
        m_Shaders[name] = shader;
    }

    const Ref<Shader>& ShaderLibrary::Load(const std::string& name,
                                           const std::string& vertexSrc,
                                           const std::string& fragmentSrc,
                                           const std::string& mode)
    {
        auto shader     = Shader::Create(name, vertexSrc, fragmentSrc, mode);
        m_Shaders[name] = shader;

        return m_Shaders[name];
    }

    const Ref<Shader>& ShaderLibrary::Load(const std::string& name,
                                           const std::string& vertexSrc,
                                           const std::string& geometrySrc,
                                           const std::string& fragmentSrc,
                                           const std::string& mode)
    {
        auto shader     = Shader::Create(name, vertexSrc, geometrySrc, fragmentSrc, mode);
        m_Shaders[name] = shader;

        return m_Shaders[name];
    }

    const Ref<Shader>& ShaderLibrary::Get(const std::string& name) { return m_Shaders[name]; }
} // namespace Engine