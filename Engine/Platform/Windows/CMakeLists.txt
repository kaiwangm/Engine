add_library(
    Platform_Windows
    STATIC
    WindowsWindow.cpp
)

target_link_libraries(Platform_Windows PRIVATE Core)
target_link_libraries(Platform_Windows PRIVATE Events)
target_link_libraries(Platform_Windows PRIVATE Log)
target_link_libraries(Platform_Windows PRIVATE Window)