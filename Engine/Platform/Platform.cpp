#include "Window.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "Windows/WindowsWindow.h"
#endif

namespace Engine {
Scope<Window> Window::Create(const WindowProps& props) {
#ifdef ENGINE_PLATFORM_WINDOWS
    return std::make_unique<WindowsWindow>(props);
#else
    return nullptr;
#endif
}
}  // namespace Engine