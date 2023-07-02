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

        template<class... Args>
        inline static void Core_Trace(Args... args)
        {
            GetCoreLogger()->trace(args...);
        }

        template<class... Args>
        inline static void Core_Info(Args... args)
        {
            GetCoreLogger()->info(args...);
        }

        template<class... Args>
        inline static void Core_Warn(Args... args)
        {
            GetCoreLogger()->warn(args...);
        }

        template<class... Args>
        inline static void Core_Error(Args... args)
        {
            GetCoreLogger()->error(args...);
        }

        template<class... Args>
        inline static void Core_Critical(Args... args)
        {
            GetCoreLogger()->critical(args...);
        }

        template<class... Args>
        inline static void Trace(Args... args)
        {
            GetClientLogger()->trace(args...);
        }

        template<class... Args>
        inline static void Info(Args... args)
        {
            GetClientLogger()->info(args...);
        }

        template<class... Args>
        inline static void Warn(Args... args)
        {
            GetClientLogger()->warn(args...);
        }

        template<class... Args>
        inline static void Error(Args... args)
        {
            GetClientLogger()->error(args...);
        }

        template<class... Args>
        inline static void Critical(Args... args)
        {
            GetClientLogger()->critical(args...);
        }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;

        static Ref<std::ostringstream> s_OString;
    };
} // namespace Engine
