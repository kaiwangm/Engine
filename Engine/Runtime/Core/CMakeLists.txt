add_library(
    Core
    STATIC
    Layer/LayerStack.cpp
    Log/Log.cpp
)

target_link_libraries(
    Core 
    PRIVATE 
    spdlog::spdlog 
    spdlog::spdlog_header_only
)

target_include_directories(Core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})