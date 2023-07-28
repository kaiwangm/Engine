#pragma once
#include <Engine/Runtime/GameFramework/StaticMesh/UChunkArrayWorldComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>

namespace Engine
{
    class AChunkArrayWorld final : public AActor
    {
    private:
        UChunkArrayWorldComponent* m_ChunkComponent;

    public:
        AChunkArrayWorld(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_ChunkComponent = m_World->RegisterComponents<UChunkArrayWorldComponent>(m_EntityHandle);
            m_ChunkComponent->SetOwner(this);
        }
    };
}; // namespace Engine