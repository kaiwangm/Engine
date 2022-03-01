add_library(
    Application
    STATIC
    Application.cpp
)

target_link_libraries(
    Application 
    PRIVATE 
    Core 
    Gui 
)

target_include_directories(Application INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})