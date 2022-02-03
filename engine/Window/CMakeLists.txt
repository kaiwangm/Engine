project(Window LANGUAGES CXX)

IF (WIN32)
    add_library(
        ${PROJECT_NAME}
        STATIC
        WIndows/WindowsWindow.cpp
        Window.cpp
    )
ENDIF ()