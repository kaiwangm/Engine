cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_subdirectory(engine)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC engine)


