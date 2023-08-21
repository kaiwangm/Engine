#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Core/Log/Log.h>
#include <Engine/Runtime/Renderer/Renderer.h>
#include <Engine/Runtime/GameFramework/Pawn/UPawnComponent.h>
#include <Engine/Runtime/GameFramework/Animation/USkinnedMeshComponent.h>
#include <opencv2/opencv.hpp>

#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/base/containers/vector.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/vec_float.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/blending_job.h>
#include <ozz/base/io/archive.h>
#include <ozz/base/io/stream.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/base/maths/vec_float.h>
#include <ozz/base/maths/quaternion.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/span.h>

namespace hnswlib
{
    template<typename dist_t>
    class HierarchicalNSW;
} // namespace hnswlib

namespace Engine
{
    class UMotionMatchingComponent : public UComponent
    {
    private:
        UStaticMeshComponent m_TrajectoryStaticMesh;
        float                m_nowTime = 0.0f;

    private:
        int   m_TrajectorySampleNum  = 10;
        float m_TrajectorySampleStep = 0.15f;

        // KnnResult m_nowAnimationClipKnnResult {0, 0.0f, 0.0f};
        static constexpr int  m_MaxSearchResultNum = 8;
        std::deque<KnnResult> m_nowAnimationClipKnnResults;

        std::vector<Ref<USkinnedMeshComponent>> m_SkinnedMeshArray;
        std::vector<TrajectoryPointArray>       m_TrajectoryPointArrays;

        static constexpr int dimRawTrajectory = (10 + 10) * 10;
        static constexpr int dimRawPose       = 138 * 6;
        static constexpr int dimRawPawn       = 7;
        static constexpr int dimTrajectory    = 150;
        static constexpr int dimPose          = 150;
        static constexpr int dimPawn          = 5;
        static constexpr int dim              = dimTrajectory + dimPose + dimPawn + 1; // Dimension of the elements
        float                weightTrajectory = 0.70f;
        float                weightPose       = 1.50f;
        float                weightPawn       = 1.80f;
        float                weightPhase      = 0.0f;

        static constexpr int                     dimPoseFeature = 138 * 3;
        static constexpr int                     dimDeepPhase   = 5 * 2;
        std::vector<ManiflowArray<dimDeepPhase>> m_ManiflowArrays;

        cv::PCA pcaTrajectory;
        cv::PCA pcaPose;
        cv::PCA pcaPawn;

        hnswlib::HierarchicalNSW<float>*    alg_hnsw;
        std::vector<std::array<float, dim>> data_raw;
        std::vector<KnnResult>              m_SearchResults;

        glm::quat m_RootRotation;

    private:
        void Initialize();
        void SaveAsJson(const std::vector<std::array<int, 2>>&        nowClipFrameIndexs,
                        const std::vector<std::vector<JointFeature>>& nowJointFeatures,
                        const std::string&                            path);
        void LoadPhaseManifold(const std::string& path);

    public:
        UMotionMatchingComponent(const std::string&              skeletonPath,
                                 const std::vector<std::string>& animationPaths,
                                 const std::string&              meshPath) :
            m_TrajectoryStaticMesh("Assets/Editor/Object/apawn/trajectory.obj")
        {
            m_nowAnimationClipKnnResults = std::deque<KnnResult>(m_MaxSearchResultNum, KnnResult {0, 0.0f, 0.0f});
            for (int i = 0; i < animationPaths.size(); i++)
            {
                m_SkinnedMeshArray.push_back(
                    std::make_shared<USkinnedMeshComponent>(skeletonPath, animationPaths[i], meshPath));
                // m_SkinnedMeshArray[i]->SetUseRootMotion(true);
            }

            Initialize();
        }
        ~UMotionMatchingComponent() {}

        void DrawTrajectory(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform);

        void Update0(float nowtime, float deltaTime)
        {
            int                    m_nowAnimationClip = 0;
            USkinnedMeshComponent* m_SkinnedMesh      = m_SkinnedMeshArray[m_nowAnimationClip].get();
            float                  frameTime          = m_SkinnedMesh->GetFrameTime();

            float ratio = (fmod(nowtime, frameTime)) / frameTime;
            m_SkinnedMesh->Update(ratio);
            m_nowTime = nowtime;
        }

        USkinnedMeshComponent& GetSkinnedMeshComponent0Ref()
        {
            int                    m_nowAnimationClip = 0;
            USkinnedMeshComponent* m_SkinnedMesh      = m_SkinnedMeshArray[m_nowAnimationClip].get();
            return *m_SkinnedMesh;
        }

        float GetFrameTime()
        {
            int                    m_nowAnimationClip = m_nowAnimationClipKnnResults[0].index;
            USkinnedMeshComponent* m_SkinnedMesh      = m_SkinnedMeshArray[m_nowAnimationClip].get();
            float                  frameTime          = m_SkinnedMesh->GetFrameTime();
            return frameTime;
        }

        void SetNowAnimationClipKnnResult(const KnnResult& knnResult) { m_nowAnimationClipKnnResults[0] = knnResult; }

        void AddNowAnimationClipKnnResult(const KnnResult& knnResult)
        {
            m_nowAnimationClipKnnResults.push_front(knnResult);
            if (m_nowAnimationClipKnnResults.size() > m_MaxSearchResultNum)
                m_nowAnimationClipKnnResults.pop_back();
        }

        void UpdateNowAnimationClipKnnResult(float deltaTime)
        {
            for (int i = 1; i < m_MaxSearchResultNum; ++i)
            {
                int                    m_nowAnimationClip = m_nowAnimationClipKnnResults[i].index;
                USkinnedMeshComponent* m_SkinnedMesh      = m_SkinnedMeshArray[m_nowAnimationClip].get();
                float                  frameTime          = m_SkinnedMesh->GetFrameTime();

                float addRatio = deltaTime / frameTime;
                m_nowAnimationClipKnnResults[i].nowRatio += addRatio;
            }
        }

        ozz::vector<ozz::math::Float4x4> GetNowAnimationClipModels()
        {
            ozz::vector<ozz::math::SoaTransform> locals_ary[m_MaxSearchResultNum];

            // get local transform
            for (int i = 0; i < m_MaxSearchResultNum; ++i)
            {
                int                    m_nowAnimationClip = m_nowAnimationClipKnnResults[i].index;
                USkinnedMeshComponent* skinnedMesh        = m_SkinnedMeshArray[m_nowAnimationClip].get();
                skinnedMesh->GetLocals(locals_ary[i], m_nowAnimationClipKnnResults[i].nowRatio);
            }

            ozz::animation::BlendingJob::Layer layers[m_MaxSearchResultNum];

            for (int i = 0; i < m_MaxSearchResultNum; ++i)
            {
                layers[i].transform = ozz::make_span(locals_ary[i]);
                if (i == 0)
                {
                    layers[i].weight = 0.30f;
                }
                else
                {
                    layers[i].weight = 0.70f / (m_MaxSearchResultNum - 1);
                }
            }

            int                    m_nowAnimationClip = m_nowAnimationClipKnnResults[0].index;
            USkinnedMeshComponent* skinnedMesh        = m_SkinnedMeshArray[m_nowAnimationClip].get();

            ozz::vector<ozz::math::SoaTransform> blended_locals_;
            ozz::animation::BlendingJob          blend_job;
            blended_locals_.resize(skinnedMesh->GetSkeletonRef().num_soa_joints());
            blend_job.threshold = 0.1f;
            blend_job.layers    = layers;
            blend_job.rest_pose = skinnedMesh->GetSkeletonRef().joint_rest_poses();
            blend_job.output    = make_span(blended_locals_);

            if (!blend_job.Run())
            {
                Log::Error("Failed to blend animation.");
            }

            // {
            //     glm::quat rootRotation;
            //     {
            //         __m128 x = blended_locals_[0].rotation.x;
            //         __m128 y = blended_locals_[0].rotation.y;
            //         __m128 z = blended_locals_[0].rotation.z;
            //         __m128 w = blended_locals_[0].rotation.w;

            //         rootRotation.x = _mm_cvtss_f32(x);
            //         rootRotation.y = _mm_cvtss_f32(y);
            //         rootRotation.z = _mm_cvtss_f32(z);
            //         rootRotation.w = _mm_cvtss_f32(w);
            //     }
            //     glm::vec3 euler           = glm::eulerAngles(rootRotation);
            //     // euler.x                   = 0.0f;
            //     // euler.y                   = 0.0f;
            //     // euler.z                   = 0.0f;
            //     glm::quat newRootRotation = glm::quat(euler);
            //     {
            //         __m128 x = _mm_set_ss(newRootRotation.x);
            //         __m128 y = _mm_set_ss(newRootRotation.y);
            //         __m128 z = _mm_set_ss(newRootRotation.z);
            //         __m128 w = _mm_set_ss(newRootRotation.w);

            //         blended_locals_[0].rotation.x = x;
            //         blended_locals_[0].rotation.y = y;
            //         blended_locals_[0].rotation.z = z;
            //         blended_locals_[0].rotation.w = w;
            //     }
            // }

            ozz::vector<ozz::math::Float4x4> models_;
            ozz::animation::LocalToModelJob  ltm_job;
            models_.resize(skinnedMesh->GetSkeletonRef().num_joints());
            ltm_job.skeleton = &skinnedMesh->GetSkeletonRef();
            ltm_job.input    = make_span(blended_locals_);
            ltm_job.output   = make_span(models_);

            // Runs ltm job.
            if (!ltm_job.Run())
            {
                Log::Error("Failed to convert local space animation to model space.");
            }

            return models_;
        }

        glm::quat GetRootRotation() { return m_RootRotation; }
        void      SetRootRotation(glm::quat rootRotation) { m_RootRotation = rootRotation; }

    public:
        KnnResult Search(const std::deque<TrajectoryPoint>&       trajecotryPointsBack,
                         const std::deque<TrajectoryPoint>&       trajecotryPointsForward,
                         const std::vector<std::array<float, 7>>& nowPose,
                         const float                              nowPhase,
                         const std::vector<JointFeature>&         nowJointFeature);

        USkinnedMeshComponent& GetNowSkinnedMeshComponentRef();
        glm::vec3              GetVelocity(float delta);
    };
} // namespace Engine