IF (WIN32)
    add_subdirectory(OpenGL)
    add_subdirectory(Windows)

    add_library(
        Platform
        STATIC
        Platform.cpp
    )

    target_link_libraries(Platform PRIVATE Core)
    target_link_libraries(Platform PRIVATE Events)
    target_link_libraries(Platform PRIVATE Platform_Windows)
    target_link_libraries(Platform PRIVATE Platform_OpenGL)
    target_link_libraries(Platform PRIVATE Renderer)
ENDIF ()

target_include_directories(Platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})