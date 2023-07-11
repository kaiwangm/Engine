add_library(
    GameFramework
    STATIC
    Camera/Camera.cpp
    DataUtil/Mesh.cpp
    DataUtil/StaticMesh.cpp
    DataUtil/Animation.cpp
    DataUtil/PointCloud.cpp
    Common/UWorld.cpp
    Animation/USkeletonComponent.cpp
    Pawn/UPawnComponent.cpp
    Animation/USkinnedMeshComponent.cpp
    Animation/ozz/mesh.cc
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
    ozz-animation
)

target_link_libraries(GameFramework PRIVATE ${PCL_LIBRARIES} OpenVDB::openvdb)

target_include_directories(GameFramework INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})