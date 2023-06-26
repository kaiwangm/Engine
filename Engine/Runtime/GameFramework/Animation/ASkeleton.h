#pragma once
#include "USkeletonComponent.h"
#include "../Common/AActor.h"

namespace Engine
{
    class ASkeleton final : public AActor
    {
    private:
        USkeletonComponent* m_Skeleton;

    public:
        ASkeleton(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_Skeleton = m_World->RegisterComponents<USkeletonComponent>(m_EntityHandle);
            m_Skeleton->SetOwner(this);
        }
    };
} // namespace Engine