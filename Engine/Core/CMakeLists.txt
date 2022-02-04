add_library(
    Core
    STATIC
    Core.cpp
    EntryPoint.h
)

target_include_directories(Core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})