cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

IF (WIN32)
	MESSAGE(STATUS "Now is windows")
    add_definitions(-DENGINE_PLATFORM_WINDOWS)
ENDIF ()

add_subdirectory(engine)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC engine)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
