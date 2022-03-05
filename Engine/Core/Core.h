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

#include <functional>
#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/core.h>

#ifdef ENGINE_PLATFORM_WINDOWS
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

namespace Engine {
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;
}  // namespace Engine

#include "Log.h"
#include "Timer.h"
#include "Events/Event.h"

