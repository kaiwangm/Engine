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