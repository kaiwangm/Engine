#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Core/Log/Log.h>
#include <Engine/Runtime/Renderer/Renderer.h>
#include <Engine/Runtime/GameFramework/Pawn/UPawnComponent.h>
#include <Engine/Runtime/GameFramework/Animation/USkinnedMeshComponent.h>

namespace Engine
{
    class UMotionMatchingComponent : public UComponent
    {
    private:
        UStaticMeshComponent m_TrajectoryStaticMesh;

    private:
        int   m_TrajectorySampleNum  = 15;
        float m_TrajectorySampleStep = 0.15f;
        float m_nowTime              = 0.0f;

        USkinnedMeshComponent m_SkinnedMesh;

        float                m_FrameTime;
        float                m_SampleStep;
        TrajectoryPointArray m_TrajectoryPointArray;

    private:
        void Initialize()
        {
            m_FrameTime  = m_SkinnedMesh.GetFrameTime();
            m_SampleStep = 1.0f / 24.0f;
            for (float nowtime = 0.0f; nowtime < m_FrameTime; nowtime += m_SampleStep)
            {
                float ratio = (fmod(nowtime, m_FrameTime)) / m_FrameTime;

                m_SkinnedMesh.Update(ratio);

                TrajectoryPoint point {
                    m_SkinnedMesh.GetNowRootPosition(),
                    m_SkinnedMesh.GetNowRootOrientation(),
                    m_SampleStep,
                };

                m_TrajectoryPointArray.push_back(point);
            }
        }

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
    };
} // namespace Engine