set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Editor)

# add_definitions(-DENGINE_USE_ENTRYPOINT)
add_executable(Editor main.cpp)

target_link_libraries(Editor PRIVATE Engine)


add_custom_command(
    TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ENGINE_ROOT_DIR}/Assets/Editor
    $<TARGET_FILE_DIR:Editor>/Assets/Editor
)
