add_library(
    OpenGL_Imgui
    STATIC
    imgui_impl_opengl3.cpp
    imgui_impl_glfw.cpp
)

target_link_libraries(
    OpenGL_Imgui 
    PRIVATE 
    Core 
)

target_link_libraries(
    OpenGL_Imgui  
    PRIVATE 
    imgui::imgui 
    glad::glad 
    glfw
)