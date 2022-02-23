#include "Log.h"

namespace Engine
{
    Ref<spdlog::logger> Log_Imp::s_CoreLogger;
    Ref<spdlog::logger> Log_Imp::s_ClientLogger;

    void Log_Imp::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
        s_ClientLogger->set_level(spdlog::level::trace);
    }

    void Log::Init()
    {
        Log_Imp::Init();
    }
}