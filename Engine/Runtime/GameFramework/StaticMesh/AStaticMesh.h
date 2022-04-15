#pragma once
#include "UStaticMeshComponent.h"
#include "../Common/AActor.h"

namespace Engine {
class AStaticMesh final : public AActor {
   private:
    UStaticMeshComponent* m_Mesh;

   public:
    AStaticMesh(UWorld* world, entt::entity handle, const std::string& name)
        : AActor(world, handle, name) {
        m_Mesh =
            m_World->RegisterComponents<UStaticMeshComponent>(m_EntityHandle);
    }
    AStaticMesh(UWorld* world, entt::entity handle, const std::string& name,
                const std::string path)
        : AActor(world, handle, name) {
        m_Mesh = m_World->RegisterComponents<UStaticMeshComponent>(
            m_EntityHandle, path);
    }
};
};  // namespace Engine