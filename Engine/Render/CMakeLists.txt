add_library(
    Render
    GraphicsContext.cpp
    Shader.cpp
)

target_link_libraries(Render PRIVATE Core)
target_include_directories(Render INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})