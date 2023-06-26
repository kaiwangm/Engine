add_library(
    Utils
    OBJECT
    Spare.cpp
    Octree.cpp
    hilbert.c
)

target_link_libraries(Utils PRIVATE Core xtensor xtensor::optimize)

target_include_directories(Utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})