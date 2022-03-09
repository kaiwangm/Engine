cmake_minimum_required(VERSION 3.22.0)
project(EngineProject VERSION 0.1.0)
project(EngineProject LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

IF (WIN32)
	MESSAGE(STATUS "Now is windows")
    add_definitions(-DENGINE_PLATFORM_WINDOWS)
ENDIF ()

find_package(spdlog CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

include (GenerateExportHeader)
include(CTest)

add_subdirectory(Engine)
add_subdirectory(Example)
add_subdirectory(Test)

enable_testing()
