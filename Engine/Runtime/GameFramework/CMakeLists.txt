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
    Animation/UMotionMatchingComponent.cpp
    StaticMesh/UChunkComponent.cpp
    StaticMesh/UChunkArrayWorldComponent.cpp
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

target_link_libraries(GameFramework PRIVATE ${PCL_LIBRARIES} OpenVDB::openvdb hnswlib::hnswlib opencv_ml opencv_dnn opencv_core opencv_flann rapidjson)

target_include_directories(GameFramework INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})