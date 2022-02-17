add_library(
    Core
    STATIC
    Application.cpp
    Layer.cpp
    LayerStack.cpp
    Log.cpp
)

target_link_libraries(Core PRIVATE Events)
target_link_libraries(Core PRIVATE Gui)
target_link_libraries(Core PRIVATE Window)
target_link_libraries(Core PRIVATE Render)

target_include_directories(Core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})