add_library(
    ImNodes
    STATIC
    ImGuiNodes.cpp
    ImNodes.cpp
    ImNodesEz.cpp
)

target_link_libraries(
    ImNodes  
    PRIVATE 
    imgui::imgui 
    glad::glad 
    glfw
)