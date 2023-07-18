#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Core/Log/Log.h>
#include <Engine/Runtime/Renderer/Renderer.h>
#include <Engine/Runtime/GameFramework/Pawn/UPawnComponent.h>
#include <Engine/Runtime/GameFramework/Animation/USkinnedMeshComponent.h>
#include <opencv2/opencv.hpp>

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

        KnnResult m_nowAnimationClipKnnResult {0, 0.0f, 0.0f};

        std::vector<Ref<USkinnedMeshComponent>> m_SkinnedMeshArray;
        std::vector<TrajectoryPointArray>       m_TrajectoryPointArrays;

        static constexpr int dimRawTrajectory = (10 + 10) * 10;
        static constexpr int dimRawPose       = 138 * 7;
        static constexpr int dimRawPawn       = 7;
        static constexpr int dimTrajectory    = 60;
        static constexpr int dimPose          = 10;
        static constexpr int dimPawn          = 5;
        static constexpr int dim              = dimTrajectory + dimPose + dimPawn + 1; // Dimension of the elements
        float                weightTrajectory = 10.0f;
        float                weightPose       = 1.5f;
        float                weightPawn       = 15.0f;
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
            for (int i = 0; i < animationPaths.size(); i++)
            {
                m_SkinnedMeshArray.push_back(
                    std::make_shared<USkinnedMeshComponent>(skeletonPath, animationPaths[i], meshPath));
                m_SkinnedMeshArray[i]->SetUseRootMotion(true);
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
            int                    m_nowAnimationClip = m_nowAnimationClipKnnResult.index;
            USkinnedMeshComponent* m_SkinnedMesh      = m_SkinnedMeshArray[m_nowAnimationClip].get();
            float                  frameTime          = m_SkinnedMesh->GetFrameTime();
            return frameTime;
        }

        void SetNowAnimationClipKnnResult(const KnnResult& knnResult) { m_nowAnimationClipKnnResult = knnResult; }

        ozz::vector<ozz::math::Float4x4> GetNowAnimationClipModels()
        {
            int                    m_nowAnimationClip = m_nowAnimationClipKnnResult.index;
            USkinnedMeshComponent* m_SkinnedMesh      = m_SkinnedMeshArray[m_nowAnimationClip].get();

            m_SkinnedMesh->SetUseRootMotion(false);
            m_SkinnedMesh->Update(m_nowAnimationClipKnnResult.nowRatio);
            m_SkinnedMesh->SetUseRootMotion(true);

            return m_SkinnedMesh->GetModels();
        }

    public:
        KnnResult Search(const std::deque<TrajectoryPoint>&       trajecotryPointsBack,
                         const std::deque<TrajectoryPoint>&       trajecotryPointsForward,
                         const std::vector<std::array<float, 7>>& nowPose,
                         const float                              nowPhase,
                         const std::vector<JointFeature>&         nowJointFeature);
    };
} // namespace Engine