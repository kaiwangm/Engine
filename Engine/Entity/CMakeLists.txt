add_library(
    Entity
    OBJECT
    Entity.cpp
    Scene.cpp
)

target_link_libraries(
    Entity 
    PRIVATE 
    Core 
    Renderer 
    Gui
)

target_link_libraries(
    Entity 
    PRIVATE 
    EnTT::EnTT 
)

target_include_directories(Entity INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})