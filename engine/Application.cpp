#include "Application.h"
#include "Log.h"
#include <fmt/format.h>

namespace Engine
{
    Application::Application()
    {
        Engine::Log::Init();
        ENGINE_CORE_TRACE("Engine Initialization");
    }

    Application::~Application()
    {
        ENGINE_CORE_TRACE("Engine Shutdown");
    }

    void Application::Run()
    {
        for(int i = 0; i < 10; ++i)
        {
            ENGINE_CORE_INFO("Run");
        }
    }
}