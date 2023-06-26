#pragma once
#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Renderer/GraphicsContext.h>

namespace Engine
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        virtual int  Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_windowHandle;
    };
} // namespace Engine