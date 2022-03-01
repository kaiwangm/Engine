add_library(
    Gui
    STATIC
    ImGuiLayer.cpp
)

target_link_libraries(
    Gui 
    PRIVATE 
    Core 
    Platform 
    Renderer
)

target_include_directories(Gui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})