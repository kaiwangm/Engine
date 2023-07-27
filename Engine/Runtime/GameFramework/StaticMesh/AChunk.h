#pragma once
#include <Engine/Runtime/GameFramework/StaticMesh/UChunkComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>

namespace Engine
{
    class AChunk final : public AActor
    {
    private:
        UChunkComponent* m_ChunkComponent;

    public:
        AChunk(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_ChunkComponent = m_World->RegisterComponents<UChunkComponent>(m_EntityHandle);
            m_ChunkComponent->SetOwner(this);
        }

        void TickLogic(float deltaTime) { m_ChunkComponent->TickLogic(deltaTime); }
    };
}; // namespace Engine