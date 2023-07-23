#include <Engine/Runtime/Renderer/Buffer.h>
#include <Engine/Runtime/Renderer/Renderer.h>

#ifdef ENGINE_PLATFORM_OPENGL
#include <Engine/Runtime/Renderer/OpenGL_Renderer/OpenGLBuffer.h>
#endif

namespace Engine
{
    Ref<VertexBuffer> VertexBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexBuffer>();
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
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

    Ref<IndexBuffer> IndexBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLIndexBuffer>();
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
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

    Ref<FrameRenderBuffer> FrameRenderBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
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

    Ref<ShadowMapBuffer> ShadowMapBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShadowMapBuffer>();
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }

    Ref<ShadowCubeMapBuffer> ShadowCubeMapBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShadowCubeMapBuffer>();
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }

    Ref<GeometryBuffer> GeometryBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLGeometryBuffer>();
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }

    Ref<SSAOBuffer> SSAOBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLSSAOBuffer>();
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }
} // namespace Engine