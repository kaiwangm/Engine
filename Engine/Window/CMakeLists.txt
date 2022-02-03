IF (WIN32)
    add_library(
        Window
        STATIC
        WIndows/WindowsWindow.cpp
        Window.cpp
    )
ENDIF ()