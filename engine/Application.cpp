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
        ENGINE_CORE_WARN("Please implement the Run function");
    }
}