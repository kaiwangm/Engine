add_subdirectory(Events)

add_library(
    Core
    STATIC
    Layer.cpp
    LayerStack.cpp
    Log.cpp
    Window.cpp
)

target_link_libraries(
    Core 
    PUBLIC 
    Events 
)

target_link_libraries(
    Core 
    PRIVATE 
    spdlog::spdlog_header_only
)

target_include_directories(Core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})