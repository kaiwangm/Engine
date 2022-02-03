IF (WIN32)
    add_subdirectory(Windows)
    add_library(
        Window
        STATIC
        Window.cpp
    )
    target_link_libraries(Window PRIVATE Windows)
    target_include_directories(Window PUBLIC Log)
ENDIF ()