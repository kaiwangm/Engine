project(engine LANGUAGES CXX)

add_library(
    ${PROJECT_NAME}
    STATIC
    engine.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)