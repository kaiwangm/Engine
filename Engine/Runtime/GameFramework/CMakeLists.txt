add_library(
    GameFramework
    OBJECT
    Animation.cpp
    Entity.cpp
    Model.cpp
    Scene.cpp
)

target_link_libraries(
    GameFramework 
    PRIVATE 
    Core 
    Renderer 
    Gui
)

target_link_libraries(
    GameFramework 
    PRIVATE 
    EnTT::EnTT 
    assimp::assimp 
)

target_include_directories(GameFramework INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})