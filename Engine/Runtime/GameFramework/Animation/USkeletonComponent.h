#pragma once
#include "../Common/UComponent.h"
#include "../Core/Log/Log.h"
#include "../Renderer/Renderer.h"

#include "ozz/animation/runtime/skeleton.h"
#include "ozz/animation/runtime/animation.h"
#include "ozz/base/io/archive.h"
#include "ozz/base/io/stream.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"
#include "ozz/base/containers/vector.h"
#include "ozz/base/span.h"

#include "../GameFramework/StaticMesh/UStaticMeshComponent.h"

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
        UStaticMeshComponent m_Mesh;

    public:
        USkeletonComponent();
        void Update(float ratio);
        void Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform);
    };
} // namespace Engine