#include <Engine/Runtime/Renderer/OpenGL_Renderer/OpenGLContext.h>

// #define GETGLSTRING(info) std::string(reinterpret_cast<const char*>(glGetString(info)))

namespace Engine
{
    const std::string GETGLSTRING(const GLuint info)
    {
        return std::string(reinterpret_cast<const char*>(glGetString(info)));
    }

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle) {}

    int OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_windowHandle);
        int gladload_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        Log::Core_Trace("OpenGL status:");
        Log::Core_Trace("--------------");
        Log::Core_Trace(fmt::format("    Vendor: {0}", GETGLSTRING(GL_VENDOR)));
        Log::Core_Trace(fmt::format("    Render: {0}", GETGLSTRING(GL_RENDERER)));
        Log::Core_Trace(fmt::format("    Version: {0}", GETGLSTRING(GL_VERSION)));
        Log::Core_Trace("--------------");

        return gladload_status;
    }
    void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_windowHandle); };
} // namespace Engine