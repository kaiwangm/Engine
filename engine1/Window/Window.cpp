#include "../Window.h"

#ifdef ENGINE_PLATFORM_WINDOWS
    #include "Windows/WindowsWindow.h"
#endif

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