add_library(
    Log
    STATIC
    Log.cpp
)
target_link_libraries(Log PRIVATE spdlog::spdlog_header_only)