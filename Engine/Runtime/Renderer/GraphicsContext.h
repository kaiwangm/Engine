#pragma once
#include <Engine/Runtime/Core/Core.h>

namespace Engine
{
    class GraphicsContext
    {
    public:
        virtual int  Init()        = 0;
        virtual void SwapBuffers() = 0;
    };
} // namespace Engine