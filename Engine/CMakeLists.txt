set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Engine)

add_definitions(-DENGINE_BUILD_DLL)

add_subdirectory(Events)
add_subdirectory(Log)
add_subdirectory(Window)

add_library(
    Engine
    SHARED
    Application.cpp
)

GENERATE_EXPORT_HEADER(Engine           
    BASE_NAME Engine
    EXPORT_MACRO_NAME ENGINE_API
    EXPORT_FILE_NAME Core.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC
)

target_include_directories(Engine PUBLIC .)

target_link_libraries(Engine PRIVATE Events)
target_link_libraries(Engine PUBLIC Log)
target_link_libraries(Engine PRIVATE Window)

target_link_libraries(Engine PRIVATE glad::glad)
target_link_libraries(Engine PRIVATE glfw)

#install(TARGETS ${PROJECT_NAME} 
#        RUNTIME DESTINATION bin
#)