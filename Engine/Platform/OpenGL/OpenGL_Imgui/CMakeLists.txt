add_library(
    OpenGL_Imgui
    STATIC
    imgui_impl_opengl3.cpp
    imgui_impl_glfw.cpp
    imgui_freetype.cpp
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
    freetype 
    glad::glad 
    glfw 
)