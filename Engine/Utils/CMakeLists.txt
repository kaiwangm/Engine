add_library(
    Utils
    OBJECT
    Spare.cpp
    Octree.cpp
    test.cu
)

target_include_directories(Utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})