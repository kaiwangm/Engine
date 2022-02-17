#pragma once
#include "Core.h"

namespace Engine
{
    class GraphicsContext
    {
        public:
            virtual int Init() = 0;
            virtual void SwapBuffers() = 0;
    };
}