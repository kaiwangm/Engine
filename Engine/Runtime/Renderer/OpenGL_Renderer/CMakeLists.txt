include_directories(..)

add_library(
    OpenGL_Renderer
    STATIC
    OpenGLBuffer.cpp
    OpenGLContext.cpp
    OpenGLRendererAPI.cpp
    OpenGLShader.cpp
    OpenGLTexture.cpp
    OpenGLVertexArray.cpp
)

target_link_libraries(
    OpenGL_Renderer
    PRIVATE 
    Core
)

target_link_libraries(
    OpenGL_Renderer 
    PRIVATE 
    glm::glm
)