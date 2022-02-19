#include "Buffer.h"

#include "Renderer.h"

#ifdef ENGINE_PLATFORM_WINDOWS
    #include "OpenGL/OpenGLBuffer.h"
#endif

namespace Engine
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {   

        VertexBuffer* buffer = nullptr;
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None : 
                ENGINE_CORE_ERROR("RendererAPI::None is currently not supported.");
                break;
            
            case RendererAPI::OpenGL : 
                buffer =  new OpenGLVertexBuffer(vertices, size);
                break;
            
            default :
                ENGINE_CORE_ERROR("RendererAPI::Unknow API.");
                break;
        }

        return buffer;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        IndexBuffer* buffer = nullptr;
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None: 
                ENGINE_CORE_ERROR("RendererAPI::None is currently not supported.");
                break;
            
            case RendererAPI::OpenGL: 
                buffer =  new OpenGLIndexBuffer(indices, count);
                break;
            
            default :
                ENGINE_CORE_ERROR("RendererAPI::Unknow API.");
                break;
        }

        return buffer;
    }
}