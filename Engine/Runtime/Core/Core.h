#pragma once

/*
#ifdef ENGINE_PLATFORM_WINDOWS
#ifdef ENGINE_BUILD_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
#error Engine only support Windows!
#endif
*/

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

#include <functional>
#include <cstdint>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/core.h>

#ifdef ENGINE_PLATFORM_WINDOWS
#define NOMINMAX
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

namespace Engine
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using WeakRef = std::weak_ptr<T>;
} // namespace Engine

#include <Engine/Runtime/Core/Events/Event.h>
#include <Engine/Runtime/Core/Log/Log.h>
#include <Engine/Runtime/Core/Timer/Timer.h>
