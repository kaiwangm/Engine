#include "VertexArray.h"

#include "Renderer.h"
#ifdef ENGINE_PLATFORM_WINDOWS
    #include "OpenGL/OpenGLVertexArray.h"
#endif


namespace Engine
{
    VertexArray* VertexArray::Create()
    {
        VertexArray* array = nullptr;
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None : 
                ENGINE_CORE_ERROR("RendererAPI::None is currently not supported.");
                break;
            
            case RendererAPI::OpenGL : 
                array =  new OpenGLVertexArray();
                break;
            
            default :
                ENGINE_CORE_ERROR("RendererAPI::Unknow API.");
                break;
        }

        return array;
    }
}