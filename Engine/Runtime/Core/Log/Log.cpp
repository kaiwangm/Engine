#include <Engine/Runtime/Core/Log/Log.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace Engine
{
    Ref<spdlog::logger>     Log::s_CoreLogger;
    Ref<spdlog::logger>     Log::s_ClientLogger;
    Ref<std::ostringstream> Log::s_OString;

    void Log::Init()
    {
        s_OString = std::make_shared<std::ostringstream>();

        auto sink1 = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto sink2 = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("Logs/Log.txt", 1024 * 1024 * 5, 3);
        auto sink3 = std::make_shared<spdlog::sinks::ostream_sink_mt>(*s_OString);
        std::vector<spdlog::sink_ptr> sinks = {sink1, sink2, sink3};

        std::string patten = "[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v";
        spdlog::init_thread_pool(8192, 1);

        s_CoreLogger = std::make_shared<spdlog::async_logger>(
            "Engine", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        s_CoreLogger->set_pattern(patten);
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::async_logger>(
            "Client", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        s_ClientLogger->set_pattern(patten);
        s_ClientLogger->set_level(spdlog::level::trace);
    }

    void Log::Delete()
    {
        s_CoreLogger->flush();
        s_ClientLogger->flush();
        spdlog::drop_all();
    }
} // namespace Engine