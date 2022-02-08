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