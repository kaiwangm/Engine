#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Core/Log/Log.h>
#include <Engine/Runtime/Renderer/Renderer.h>

#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/base/containers/vector.h>
#include <ozz/base/maths/simd_math.h>
#include "ozz/base/maths/vec_float.h"

#include <Engine/Runtime/GameFramework/StaticMesh/UStaticMeshComponent.h>
#include <vector>

namespace Engine
{
    class USkeletonComponent : public UComponent
    {
    private:
        ozz::animation::Skeleton             skeleton;
        ozz::animation::Animation            animation;
        ozz::vector<ozz::math::SoaTransform> locals;
        ozz::vector<ozz::math::Float4x4>     models;
        int                                  num_soa_joints;
        int                                  num_joints;
        std::vector<int>                     parents;
        ozz::math::Float3                    color;

    private:
        UStaticMeshComponent m_Joints;
        UStaticMeshComponent m_Bones;

    private:
        static bool loadSkeleton(const std::string filename, ozz::animation::Skeleton* skeleton);
        static bool loadAnimation(const std::string filename, ozz::animation::Animation* animation);

    public:
        USkeletonComponent();
        ~USkeletonComponent();

    public:
        void Update(float ratio);
        void USkeletonComponent::Draw(Ref<Shader> shader,
                                      glm::mat4   vpMat,
                                      glm::mat4   transform,
                                      glm::vec3   viewPos,
                                      glm::vec3   color,
                                      glm::vec3   lightColor);

    public:
        int GetNumJoints();
    };
} // namespace Engine