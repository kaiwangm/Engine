add_library(
    Renderer
    GraphicsContext.cpp
    Shader.cpp
    Buffer.cpp
    Renderer.cpp
    VertexArray.cpp
    RenderCommand.cpp
    RendererAPI.cpp
)

target_link_libraries(Renderer PRIVATE Core)
target_link_libraries(Renderer PRIVATE Platform)
target_link_libraries(Renderer PRIVATE glm::glm)

target_include_directories(Renderer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})