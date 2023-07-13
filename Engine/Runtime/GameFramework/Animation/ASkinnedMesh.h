#pragma once
#include <Engine/Runtime/GameFramework/Animation/USkinnedMeshComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>
#include <Engine/Runtime/GameFramework/Material/MBasicPbr.h>

namespace Engine
{
    class ASkinnedMesh final : public AActor
    {
    private:
        USkinnedMeshComponent* m_SkinnedMesh;

    public:
        ASkinnedMesh(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_SkinnedMesh = m_World->RegisterComponents<USkinnedMeshComponent>(m_EntityHandle);
            m_SkinnedMesh->SetOwner(this);
        }

        ASkinnedMesh(UWorld*            world,
                     entt::entity       handle,
                     const std::string& name,
                     const std::string& skeletonPath,
                     const std::string& animationPath,
                     const std::string& meshPath) :
            AActor(world, handle, name)
        {
            m_SkinnedMesh = m_World->RegisterComponents<USkinnedMeshComponent>(
                m_EntityHandle, skeletonPath, animationPath, meshPath);
            m_SkinnedMesh->SetOwner(this);
        }
    };
} // namespace Engine