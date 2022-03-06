add_subdirectory(ImNodes)

add_library(
    Gui
    STATIC
    DockSpaceLayer.cpp
    GuiCommand.cpp
    ImGuiLayer.cpp
    ImGuiLog.cpp
)

target_link_libraries(
    Gui 
    PRIVATE 
    Core 
    Platform 
    Renderer
)

target_link_libraries(
    Gui 
    PRIVATE 
    ImNodes
)

target_link_libraries(
    Gui 
    PRIVATE 
    imgui::imgui 
)

target_include_directories(Gui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})