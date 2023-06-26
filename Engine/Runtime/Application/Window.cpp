#include <Engine/Runtime/Application/Window.h>

#ifdef ENGINE_PLATFORM_WINDOWS
#include <Engine/Runtime/Application/Windows_Window/WindowsWindow.h>
#endif

namespace Engine
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
#ifdef ENGINE_PLATFORM_WINDOWS
        return std::make_unique<WindowsWindow>(props);
#else
        return nullptr;
#endif
    }
} // namespace Engine