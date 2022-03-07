add_library(
    Events
    STATIC
    Event.cpp
)

target_link_libraries(
    Events 
    PRIVATE 
    Core
)

target_link_libraries(
    Events 
    PRIVATE 
    spdlog::spdlog 
    spdlog::spdlog_header_only
)

target_include_directories(Events INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})