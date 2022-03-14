add_subdirectory(OpenGL_Renderer)

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
    Core 
    OpenGL_Renderer
)

target_link_libraries(
    Renderer 
    PRIVATE 
    glm::glm
)

target_include_directories(Renderer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})