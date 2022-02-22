add_library(
    Platform_OpenGL
    STATIC
    imgui_impl_opengl3.cpp
    imgui_impl_glfw.cpp
    OpenGLContext.cpp
    OpenGLBuffer.cpp
    OpenGLVertexArray.cpp
)

target_link_libraries(Platform_OpenGL PRIVATE Core)
target_link_libraries(Platform_OpenGL PRIVATE imgui::imgui)
target_link_libraries(Platform_OpenGL PRIVATE Renderer)

target_include_directories(Platform_OpenGL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})