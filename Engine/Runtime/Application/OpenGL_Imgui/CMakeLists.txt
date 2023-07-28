include_directories(..)

add_library(
    Gui_OpenGL_Imgui
    OBJECT
    imgui_impl_opengl3.cpp
    imgui_impl_glfw.cpp
)

target_link_libraries(
    Gui_OpenGL_Imgui
    PRIVATE
    Core
)

target_link_libraries(
    Gui_OpenGL_Imgui
    PRIVATE
    imgui::imgui
    glad::glad
    glfw
)