add_library(
    Gui
    STATIC
    ImGuiLayer.cpp
)

target_link_libraries(
    Gui 
    PRIVATE 
    Core 
    Application 
    Events 
    Platform 
    Renderer
)

target_link_libraries(
    Gui 
    PRIVATE 
    imgui::imgui
)

target_include_directories(Gui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})