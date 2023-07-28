include_directories(..)

add_library(
    Windows_Window
    OBJECT
    WindowsWindow.cpp
)

target_link_libraries(
    Windows_Window
    PRIVATE
    Core
    Renderer
)

target_link_libraries(
    Windows_Window
    PRIVATE
    glm::glm
)

target_include_directories(Windows_Window INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})