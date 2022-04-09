#pragma once
#include "Core.h"
#include "GraphicsContext.h"

namespace Engine {
class OpenGLContext : public GraphicsContext {
   public:
    OpenGLContext(GLFWwindow* windowHandle);

    virtual int Init() override;
    virtual void SwapBuffers() override;

   private:
    GLFWwindow* m_windowHandle;
};
}  // namespace Engine