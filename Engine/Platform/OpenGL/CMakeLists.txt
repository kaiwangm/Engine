add_subdirectory(OpenGL_Imgui)
add_subdirectory(Stb)

add_library(
    Platform_OpenGL
    STATIC
    OpenGLBuffer.cpp
    OpenGLContext.cpp
    OpenGLImGuiLayer.cpp
    OpenGLRendererAPI.cpp
    OpenGLShader.cpp
    OpenGLTexture.cpp
    OpenGLVertexArray.cpp
)

target_link_libraries(
    Platform_OpenGL 
    PRIVATE 
    Core 
    Application
    Renderer
    Gui
)

target_link_libraries(
    Platform_OpenGL 
    PRIVATE 
    OpenGL_Imgui
    Stb
)

target_include_directories(Platform_OpenGL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})