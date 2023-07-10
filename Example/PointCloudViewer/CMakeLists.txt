set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/PointCloudViewer)

# add_definitions(-DENGINE_USE_ENTRYPOINT)
add_executable(PointCloudViewer main.cpp)

target_link_libraries(PointCloudViewer PRIVATE Engine)

# add_custom_command(
# TARGET Editor POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy_directory
# $<TARGET_FILE_DIR:Engine>
# $<TARGET_FILE_DIR:Editor>
# )

add_custom_command(
    TARGET PointCloudViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ENGINE_ROOT_DIR}/Engine/Editor/Assets
    $<TARGET_FILE_DIR:PointCloudViewer>/Assets/Editor
)

add_custom_command(
    TARGET PointCloudViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    $<TARGET_FILE_DIR:PointCloudViewer>/Assets/PointCloudViewer
)
