add_library(
    Utils
    OBJECT
    Spare.cpp
)

target_include_directories(Utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})