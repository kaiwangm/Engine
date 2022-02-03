project(Engine LANGUAGES CXX)

add_definitions(-DENGINE_BUILD_DLL)

include (GenerateExportHeader)

add_library(
    ${PROJECT_NAME}
    SHARED
    Application.cpp
)

GENERATE_EXPORT_HEADER(${PROJECT_NAME}           
    BASE_NAME ${PROJECT_NAME}
    EXPORT_MACRO_NAME ENGINE_API
    EXPORT_FILE_NAME Core.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}
)

add_subdirectory(Log)
target_link_libraries(${PROJECT_NAME} PRIVATE Log)

add_subdirectory(Window)
target_link_libraries(${PROJECT_NAME} PRIVATE Window)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

install(TARGETS ${PROJECT_NAME} 
        RUNTIME DESTINATION bin
        )