add_library(
    Renderer
    STATIC
    GraphicsContext.cpp
    Shader.cpp
    Buffer.cpp
    Renderer.cpp
    VertexArray.cpp
    RenderCommand.cpp
    RendererAPI.cpp
    Camera.cpp
    Texture.cpp
)

target_link_libraries(
    Renderer 
    PRIVATE 
    glm::glm
)

IF (ENGINE_OPENGL_USE)
    add_subdirectory(OpenGL_Renderer)
    target_link_libraries(
    Renderer 
    PRIVATE 
    Core 
    OpenGL_Renderer
    )
ENDIF ()

target_include_directories(Renderer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})