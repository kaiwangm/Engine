#include "ImGuiLayer.h"

#include "Renderer.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "OpenGLImGuiLayer.h"
#endif

namespace Engine
{
    Ref<ImGuiLayer> ImGuiLayer::Create(const std::string& name)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                Log::Core_Error("RendererAPI::None is currently not supported.");
                break;

            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLImGuiLayer>(name);
                break;

            default:
                Log::Core_Error("RendererAPI::Unknow API.");
                break;
        }

        return nullptr;
    }

} // namespace Engine