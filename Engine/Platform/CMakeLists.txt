IF (WIN32)
    add_subdirectory(OpenGL)
    add_subdirectory(Windows)

    add_library(
        Platform
        STATIC
        Platform.cpp
    )

    target_link_libraries(
        Platform 
        PRIVATE 
        Core 
        Renderer
    )

    target_link_libraries(
        Platform 
        PRIVATE 
        Platform_Windows 
        Platform_OpenGL 
        glm::glm
    )
ENDIF ()

target_include_directories(Platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})