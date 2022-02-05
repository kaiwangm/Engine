add_library(
    Layer
    STATIC
    Layer.cpp
    LayerStack.cpp
    ImGuiLayer.cpp
)

target_link_libraries(Layer PRIVATE Application)
target_link_libraries(Layer PRIVATE Core)
target_link_libraries(Layer PRIVATE Log)
target_link_libraries(Layer PRIVATE Events)
target_link_libraries(Layer PRIVATE Platform)
target_link_libraries(Layer PRIVATE Window)

target_include_directories(Layer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})