IF (WIN32)
    add_subdirectory(Windows)

    add_library(
        Platform
        STATIC
        Platform.cpp
    )

    target_link_libraries(Platform PRIVATE Core)
    target_link_libraries(Platform PRIVATE Events)
    target_link_libraries(Platform PRIVATE Log)
    target_link_libraries(Platform PRIVATE Window)
    target_link_libraries(Platform PRIVATE Platform_Windows)
ENDIF ()

target_include_directories(Platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})