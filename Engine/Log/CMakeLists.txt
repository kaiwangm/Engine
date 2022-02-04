add_library(
    Log
    STATIC
    Log.cpp
)

target_link_libraries(Log PRIVATE Core)
target_link_libraries(Log PRIVATE spdlog::spdlog_header_only)

target_include_directories(Log INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})