#include "Window.h"

#include "Platform.h"

namespace Engine
{
    Window* Window::Create(const WindowProps& props)
	{
	#ifdef ENGINE_PLATFORM_WINDOWS
		return new WindowsWindow(props);
	#else
		return nullptr;
	#endif
	}
}