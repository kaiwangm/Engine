set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Voxelcraft)

# add_definitions(-DENGINE_USE_ENTRYPOINT)
add_executable(Voxelcraft main.cpp)

target_link_libraries(Voxelcraft PRIVATE Engine)

# add_custom_command(
# TARGET Editor POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy_directory
# $<TARGET_FILE_DIR:Engine>
# $<TARGET_FILE_DIR:Editor>
# )

add_custom_command(
    TARGET Voxelcraft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ENGINE_ROOT_DIR}/Assets/Editor
    $<TARGET_FILE_DIR:Voxelcraft>/Assets/Editor
)

add_custom_command(
    TARGET Voxelcraft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ENGINE_ROOT_DIR}/Assets/Voxelcraft
    $<TARGET_FILE_DIR:Voxelcraft>/Assets/Voxelcraft
)
