add_library(
    Gui
    STATIC
    DockSpaceLayer.cpp
    GuiCommand.cpp
    ImGuiLog.cpp
    ImNodes/ImGuiNodes.cpp
    ImNodes/ImNodes.cpp
    ImNodes/ImNodesEz.cpp
)

target_link_libraries(
    Gui 
    PRIVATE 
    Core 
    Renderer
)

target_link_libraries(
    Gui 
    PRIVATE 
    imgui::imgui 
    imguizmo::imguizmo
)

target_include_directories(Gui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})