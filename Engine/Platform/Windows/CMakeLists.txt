add_library(
    Platform_Windows
    STATIC
    WindowsWindow.cpp
    WindowsInput
)

target_link_libraries(Platform_Windows PRIVATE Core)
target_link_libraries(Platform_Windows PRIVATE Events)
target_link_libraries(Platform_Windows PRIVATE Window)
target_link_libraries(Platform_Windows PRIVATE Gui)

target_link_libraries(Platform_Windows PRIVATE Platform_OpenGL)
target_link_libraries(Platform_Windows PRIVATE Render)

target_include_directories(Platform_Windows INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})