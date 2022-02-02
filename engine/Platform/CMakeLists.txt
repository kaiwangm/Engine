project(Platform LANGUAGES CXX)

IF (WIN32)
    add_library(
        ${PROJECT_NAME}
        STATIC
        WIndows/WindowsWindow.cpp
    )

    find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
ENDIF ()