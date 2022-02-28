add_library(
    Platform_Windows
    STATIC
    WindowsWindow.cpp
    WindowsInput
)

target_link_libraries(
    Platform_Windows 
    PRIVATE 
    Core 
    Application 
    Gui 
    Platform_OpenGL 
    Renderer
)

target_include_directories(Platform_Windows INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})