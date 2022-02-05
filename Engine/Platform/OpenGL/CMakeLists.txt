add_library(
    Platform_OpenGL
    STATIC
    imgui_impl_opengl3.cpp
)

target_link_libraries(Platform_OpenGL PRIVATE imgui::imgui)