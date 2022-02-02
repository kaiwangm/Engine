project(engine LANGUAGES CXX)

add_definitions(-DENGINE_BUILD_DLL)

include (GenerateExportHeader)

add_library(
    ${PROJECT_NAME}
    SHARED
    Window.cpp
    Application.cpp
    Log.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Platform)
target_link_libraries(${PROJECT_NAME} PRIVATE Platform)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

install(TARGETS ${PROJECT_NAME} 
        RUNTIME DESTINATION bin
        )