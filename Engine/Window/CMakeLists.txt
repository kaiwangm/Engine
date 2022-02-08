IF (WIN32)
    add_library(
        Window
        STATIC
        Window.cpp
    )
    
    target_link_libraries(Window PRIVATE Core)
    target_link_libraries(Window PRIVATE Events)
    target_link_libraries(Window PRIVATE Platform)

    target_include_directories(Window INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF ()