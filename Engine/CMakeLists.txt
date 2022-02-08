set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Engine)

add_definitions(-DENGINE_BUILD_DLL)

add_subdirectory(Core)
add_subdirectory(Events)
add_subdirectory(Gui)
add_subdirectory(Platform)
add_subdirectory(Window)

add_library(
    Engine
    SHARED
    Engine.cpp
)

GENERATE_EXPORT_HEADER(Engine           
    BASE_NAME Engine
    EXPORT_MACRO_NAME ENGINE_API
    EXPORT_FILE_NAME Core.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC
)

target_include_directories(Engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Engine PUBLIC Core)
target_link_libraries(Engine PUBLIC Events)
target_link_libraries(Engine PUBLIC Gui)
target_link_libraries(Engine PUBLIC Platform)
target_link_libraries(Engine PUBLIC Window)

target_link_libraries(Engine PRIVATE glad::glad)
target_link_libraries(Engine PRIVATE glfw)
target_link_libraries(Engine PUBLIC glm::glm)

#install(TARGETS ${PROJECT_NAME} 
#        RUNTIME DESTINATION bin
#)