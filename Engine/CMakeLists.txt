project(Engine LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Engine)

add_definitions(-DENGINE_BUILD_DLL)

add_subdirectory(Log)
add_subdirectory(Window)

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

target_link_libraries(${PROJECT_NAME} PRIVATE Log)
target_link_libraries(${PROJECT_NAME} PRIVATE Window)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

#install(TARGETS ${PROJECT_NAME} 
#        RUNTIME DESTINATION bin
#)