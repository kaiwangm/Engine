#pragma once

#include <Engine/Runtime/Core/Core.h>
#include <spdlog/spdlog.h>

namespace Engine
{
    class Log
    {
    public:
        static void Init();
        static void Delete();

        inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

        inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        inline static Ref<std::ostringstream>& GetOString() { return s_OString; }

        inline static void Core_Trace(const std::string& log) { GetCoreLogger()->trace(log); }

        inline static void Core_Info(const std::string& log) { GetCoreLogger()->info(log); }

        inline static void Core_Warn(const std::string& log) { GetCoreLogger()->warn(log); }

        inline static void Core_Error(const std::string& log) { GetCoreLogger()->error(log); }

        inline static void Core_Critical(const std::string& log) { GetCoreLogger()->critical(log); }

        inline static void Trace(const std::string& log) { GetClientLogger()->trace(log); }

        inline static void Info(const std::string& log) { GetClientLogger()->info(log); }

        inline static void Warn(const std::string& log) { GetClientLogger()->warn(log); }

        inline static void Error(const std::string& log) { GetClientLogger()->error(log); }

        inline static void Critical(const std::string& log) { GetClientLogger()->critical(log); }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;

        static Ref<std::ostringstream> s_OString;
    };
} // namespace Engine
