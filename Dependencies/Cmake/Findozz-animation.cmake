add_library(
    ozz-animation STATIC
    Dependencies/ozz-animation/build/src_fused/ozz_animation_offline.cc
    Dependencies/ozz-animation/build/src_fused/ozz_animation_tools.cc
    Dependencies/ozz-animation/build/src_fused/ozz_animation.cc
    Dependencies/ozz-animation/build/src_fused/ozz_base.cc
    Dependencies/ozz-animation/build/src_fused/ozz_geometry.cc
    Dependencies/ozz-animation/build/src_fused/ozz_options.cc
)

target_include_directories(ozz-animation PUBLIC Dependencies/ozz-animation/include)
target_include_directories(ozz-animation PRIVATE Dependencies/ozz-animation/extern/jsoncpp/dist)
