#pragma once
#include "UStaticMeshComponent.h"
#include "../Common/AActor.h"

namespace Engine
{
    class AStaticMesh final : public AActor
    {
    private:
        UStaticMeshComponent* m_Mesh;

    public:
        AStaticMesh(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_Mesh = m_World->RegisterComponents<UStaticMeshComponent>(m_EntityHandle);
            m_Mesh->SetOwner(this);
        }
        AStaticMesh(UWorld* world, entt::entity handle, const std::string& name, const std::string path) :
            AActor(world, handle, name)
        {
            m_Mesh = m_World->RegisterComponents<UStaticMeshComponent>(m_EntityHandle, path);
            m_Mesh->SetOwner(this);
        }
    };
}; // namespace Engine