#pragma once
#include <Engine/Runtime/GameFramework/Common/AActor.h>
#include <Engine/Runtime/GameFramework/Pawn/UPawnComponent.h>
#include <Engine/Runtime/GameFramework/Camera/UCameraComponent.h>

#include <Engine/Runtime/GameFramework/Animation/USkinnedMeshComponent.h>
#include <Engine/Runtime/GameFramework/Animation/UMotionMatchingComponent.h>

namespace Engine
{
    class APawn final : public AActor
    {
    private:
        UPawnComponent*       m_Pawn;
        UCameraComponent*     m_Camera;
        UTrajectoryComponent* m_Trajectory;

        USkinnedMeshComponent*    m_PawnSkinnedMesh;
        UMotionMatchingComponent* m_MotionMaching;

    public:
        APawn(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_Pawn = m_World->RegisterComponents<UPawnComponent>(m_EntityHandle);
            m_Pawn->SetOwner(this);

            auto camera = std::make_shared<PerspectiveCamera>(60.0f, 1.778f, 0.1f, 800.0f);

            m_Camera = m_World->RegisterComponents<UCameraComponent>(m_EntityHandle, camera);
            m_Camera->SetOwner(this);

            m_Camera->SetPosition(glm::vec3 {0.0f, 3.0f, 5.0f});
            m_Camera->SetRotation(glm::vec3 {-0.35f, 0.0f, 0.0f});

            m_Trajectory = m_World->RegisterComponents<UTrajectoryComponent>(m_EntityHandle);
            m_Trajectory->SetOwner(this);
        }

        APawn(UWorld*                         world,
              entt::entity                    handle,
              const std::string&              name,
              const std::string&              skeletonPath,
              const std::vector<std::string>& animationPaths,
              const std::string&              meshPath) :
            AActor(world, handle, name)
        {
            m_Pawn = m_World->RegisterComponents<UPawnComponent>(m_EntityHandle);
            m_Pawn->SetOwner(this);

            auto camera = std::make_shared<PerspectiveCamera>(60.0f, 1.778f, 0.1f, 800.0f);

            m_Camera = m_World->RegisterComponents<UCameraComponent>(m_EntityHandle, camera);
            m_Camera->SetOwner(this);

            m_Camera->SetPosition(glm::vec3 {0.0f, 3.0f, 5.0f});
            m_Camera->SetRotation(glm::vec3 {-0.35f, 0.0f, 0.0f});

            m_Trajectory = m_World->RegisterComponents<UTrajectoryComponent>(m_EntityHandle);
            m_Trajectory->SetOwner(this);

            m_PawnSkinnedMesh = m_World->RegisterComponents<USkinnedMeshComponent>(
                m_EntityHandle, skeletonPath, animationPaths[0], meshPath);
            m_PawnSkinnedMesh->SetOwner(this);
            m_PawnSkinnedMesh->SetUseRootMotion(false);

            m_MotionMaching = m_World->RegisterComponents<UMotionMatchingComponent>(
                m_EntityHandle, skeletonPath, animationPaths, meshPath);
            m_MotionMaching->SetOwner(this);
        }

        UPawnComponent&           GetPawnComponentRef() { return *m_Pawn; }
        UCameraComponent&         GetCameraComponentRef() { return *m_Camera; }
        UTrajectoryComponent&     GetTrajectoryComponentRef() { return *m_Trajectory; }
        USkinnedMeshComponent&    GetSkinnedMeshComponentRef() { return *m_PawnSkinnedMesh; }
        UMotionMatchingComponent& GetMotionMatchingComponentRef() { return *m_MotionMaching; }
    };
} // namespace Engine