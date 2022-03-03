add_library(
    Octree
    STATIC
    Octree.cpp
)

target_include_directories(Octree INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})