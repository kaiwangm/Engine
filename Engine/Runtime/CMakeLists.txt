# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Engine)

add_definitions(-DENGINE_BUILD_DLL)

add_subdirectory(Core)
add_subdirectory(GameFramework)
add_subdirectory(Application)
add_subdirectory(Gui)
add_subdirectory(Renderer)
add_subdirectory(Utils)

add_library(
    Engine
    STATIC
    Engine.cpp
)

# GENERATE_EXPORT_HEADER(Engine
# BASE_NAME Engine
# EXPORT_MACRO_NAME ENGINE_API
# EXPORT_FILE_NAME Core.h
# STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC
# )
target_include_directories(Engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    Engine
    PUBLIC
    Core
    Application
    Gui
    Renderer
    Utils
    GameFramework
)

target_link_libraries(
    Engine
    PUBLIC
    glm::glm
    EnTT::EnTT
    ozz-animation
)
