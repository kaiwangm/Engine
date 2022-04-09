#include "OpenGLContext.h"

#include "Log.h"

#define GETGLSTRING(info) \
    std::string(reinterpret_cast<const char*>(glGetString(info)))

namespace Engine {
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_windowHandle(windowHandle) {}

int OpenGLContext::Init() {
    glfwMakeContextCurrent(m_windowHandle);
    int gladload_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Log::Core_Trace("OpenGL status:");
    Log::Core_Trace("    Vendor: {0}", GETGLSTRING(GL_VENDOR));
    Log::Core_Trace("    Render: {0}", GETGLSTRING(GL_RENDERER));
    Log::Core_Trace("    Version: {0}", GETGLSTRING(GL_VERSION));

    return gladload_status;
}
void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_windowHandle); };
}  // namespace Engine