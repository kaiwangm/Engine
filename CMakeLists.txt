cmake_minimum_required(VERSION 3.22)
project(EngineProject
    VERSION 0.0.6
    DESCRIPTION "My Toy Modern OpenGL Game Engine."
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(Boost_NO_WARN_NEW_VERSIONS 1)

IF(WIN32)
    MESSAGE(STATUS "Now is windows.")
    set(ENGINE_PLATFORM "Windows")
    set(ENGINE_OPENGL_USE True)
    add_definitions(-DENGINE_PLATFORM_WINDOWS)
    add_definitions(-DENGINE_PLATFORM_OPENGL)
ENDIF()

find_path(STB_INCLUDE_DIRS "stb.h")
find_package(spdlog CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(PCL CONFIG REQUIRED)
find_package(OpenVDB CONFIG REQUIRED)
find_package(hnswlib CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

include(Cmake/Findozz-animation.cmake)
include(GenerateExportHeader)
include(CTest)

add_subdirectory(Engine)
add_subdirectory(Example)
add_subdirectory(Test)

enable_testing()































