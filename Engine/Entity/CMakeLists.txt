add_library(
    Entity
    OBJECT
    Entity.cpp
    Model.cpp
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
    assimp::assimp 
)

target_include_directories(Entity INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})