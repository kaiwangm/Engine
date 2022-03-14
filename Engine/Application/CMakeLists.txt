add_subdirectory(OpenGL_Imgui)
add_subdirectory(Windows_Window)

add_library(
    Application
    STATIC
    Application.cpp
    ImGuiLayer.cpp
    Window.cpp
    OpenGLImGuiLayer.cpp
    WindowsInput.cpp
)

target_link_libraries(
    Application 
    PRIVATE 
    Core 
    Gui 
    Renderer
    Gui_OpenGL_Imgui
    Windows_Window
)

target_link_libraries(
    Application 
    PRIVATE 
    glm::glm
    imgui::imgui 
)

target_include_directories(Application INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})