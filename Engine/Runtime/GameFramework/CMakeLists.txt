add_library(
    GameFramework
    STATIC
    Camera/Camera.cpp
    DataUtil/Mesh.cpp
    Animation/Animation.cpp
    StaticMesh/StaticMesh.cpp
    Common/UWorld.cpp
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