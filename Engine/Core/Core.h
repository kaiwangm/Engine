#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
    #ifdef ENGINE_BUILD_DLL
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif
#else
    #error Engine only support Windows!
#endif

#define BIT(x) (1 << x)
#define PI 3.1415926f
#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <functional>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef ENGINE_PLATFORM_WINDOWS
    #include <windows.h>
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#endif