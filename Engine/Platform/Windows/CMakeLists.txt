add_library(
    Platform_Windows
    STATIC
    WindowsWindow.cpp
    WindowsInput.cpp
)

target_link_libraries(
    Platform_Windows 
    PRIVATE 
    Core 
    Application 
    Gui 
    Renderer
)

target_link_libraries(
    Platform_Windows 
    PRIVATE 
    Platform_OpenGL 
    glm::glm
)

target_include_directories(Platform_Windows INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})