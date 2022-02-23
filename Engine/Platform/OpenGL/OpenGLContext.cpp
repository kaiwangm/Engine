#include "OpenGLContext.h"
#include "Log.h"

# define GETGLSTRING(info) std::string(reinterpret_cast<const char*>(glGetString(info)))

namespace Engine
{
     OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
     : m_windowHandle(windowHandle)
     {

     }

    int OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_windowHandle);
		int gladload_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        ENGINE_CORE_TRACE("OpenGL Info:");
        ENGINE_CORE_TRACE("    Vendor: {0}", GETGLSTRING(GL_VENDOR));
        ENGINE_CORE_TRACE("    Render: {0}", GETGLSTRING(GL_RENDERER));
        ENGINE_CORE_TRACE("    Version: {0}", GETGLSTRING(GL_VERSION));

        return gladload_status;
    }
    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    };
}