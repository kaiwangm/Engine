set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/OctreeExample)

add_definitions(-DENGINE_USE_ENTRYPOINT)

find_package(PCL CONFIG REQUIRED)
add_subdirectory(Octree)

add_executable(OctreeExample main.cpp)

target_link_libraries(OctreeExample PRIVATE Engine)
target_link_libraries(OctreeExample PRIVATE Octree)
target_link_libraries(OctreeExample PRIVATE ${PCL_LIBRARIES})

#add_custom_command(
#    TARGET Octree POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    $<TARGET_FILE_DIR:Engine>
#    $<TARGET_FILE_DIR:Octree>
#)

add_custom_command(
    TARGET OctreeExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/Assert
            $<TARGET_FILE_DIR:OctreeExample>/Assert
)
