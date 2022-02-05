add_library(
    Application
    STATIC
    Application.cpp
)

target_link_libraries(Application PRIVATE spdlog::spdlog_header_only)
target_link_libraries(Application PRIVATE Core)
target_link_libraries(Application PRIVATE Events)
target_link_libraries(Application PRIVATE Layer)
target_link_libraries(Application PRIVATE Log)
target_link_libraries(Application PRIVATE Window)

target_include_directories(Application INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})