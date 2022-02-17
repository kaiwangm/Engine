#include "OpenGLContext.h"
#include "Log.h"

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
        ENGINE_CORE_TRACE("    Vendor: {0}", glGetString(GL_VENDOR));
        ENGINE_CORE_TRACE("    Render: {0}", glGetString(GL_RENDERER));
        ENGINE_CORE_TRACE("    Version: {0}", glGetString(GL_VERSION));

        return gladload_status;
    }
    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    };
}