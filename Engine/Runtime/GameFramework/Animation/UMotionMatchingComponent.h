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

    private:
        int   m_TrajectorySampleNum  = 10;
        float m_TrajectorySampleStep = 0.15f;
        float m_nowTime              = 0.0f;

        USkinnedMeshComponent m_SkinnedMesh;

        float                m_FrameTime;
        float                m_SampleStep;
        TrajectoryPointArray m_TrajectoryPointArray;

        static constexpr int dimRawTrajectory = (10 + 10) * 10;
        static constexpr int dimRawPose       = 42 * 7;
        static constexpr int dimRawPawn       = 7;
        static constexpr int dimTrajectory    = 50;
        static constexpr int dimPose          = 50;
        static constexpr int dimPawn          = 5;
        static constexpr int dim              = dimTrajectory + dimPose + dimPawn + 1; // Dimension of the elements
        float                weightTrajectory = 0.3f;
        float                weightPose       = 7.0f;
        float                weightPawn       = 5.0f;
        float                weightPhase      = 0.0f;

        cv::PCA pcaTrajectory;
        cv::PCA pcaPose;
        cv::PCA pcaPawn;

        hnswlib::HierarchicalNSW<float>*    alg_hnsw;
        std::vector<KnnResult>              m_SearchResult;
        std::vector<std::array<float, dim>> data_raw;

    private:
        void Initialize();

    public:
        UMotionMatchingComponent(const std::string& skeletonPath,
                                 const std::string& animationPath,
                                 const std::string& meshPath) :
            m_TrajectoryStaticMesh("Assets/Editor/Object/apawn/trajectory.obj"),
            m_SkinnedMesh(skeletonPath, animationPath, meshPath)
        {
            m_SkinnedMesh.SetUseRootMotion(true);
            Initialize();
        }
        ~UMotionMatchingComponent() {}

        void Update(float nowtime, float deltaTime)
        {
            float ratio = (fmod(nowtime, m_FrameTime)) / m_FrameTime;
            m_SkinnedMesh.Update(ratio);
            m_nowTime = nowtime;
        }

        void DrawTrajectory(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform);

        void DrawSkinnedMesh(Ref<Shader> skinnedMeshShader, glm::mat4 transform, glm::mat4 projection, glm::mat4 view)
        {
            m_SkinnedMesh.DrawSkinnedMesh(skinnedMeshShader, transform, projection, view);
        }

        USkinnedMeshComponent& GetSkinnedMeshComponentRef() { return m_SkinnedMesh; }

        float GetFrameTime() { return m_FrameTime; }

    public:
        KnnResult Search(const std::deque<TrajectoryPoint>&       trajecotryPointsBack,
                         const std::deque<TrajectoryPoint>&       trajecotryPointsForward,
                         const std::vector<std::array<float, 7>>& nowPose,
                         const float                              nowPhase);
    };
} // namespace Engine