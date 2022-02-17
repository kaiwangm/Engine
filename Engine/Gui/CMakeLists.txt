add_library(
    Gui
    STATIC
    ImGuiLayer.cpp
)

target_link_libraries(Gui PRIVATE Core)
target_link_libraries(Gui PRIVATE Events)
target_link_libraries(Gui PRIVATE Platform)
target_link_libraries(Gui PRIVATE Window)
target_link_libraries(Gui PRIVATE imgui::imgui)
target_link_libraries(Gui PRIVATE Render)
target_link_libraries(Gui PRIVATE Platform_OpenGL)

target_include_directories(Gui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})