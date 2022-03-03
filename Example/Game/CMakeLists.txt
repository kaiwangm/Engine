set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Game)

add_definitions(-DENGINE_USE_ENTRYPOINT)

add_executable(Game main.cpp)

target_link_libraries(Game PRIVATE Engine)

#add_custom_command(
#    TARGET Game POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    $<TARGET_FILE_DIR:Engine>
#    $<TARGET_FILE_DIR:Game>
#)

add_custom_command(
    TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/Assert
            $<TARGET_FILE_DIR:Game>/Assert
)
