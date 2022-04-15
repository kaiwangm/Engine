#pragma once
#include "UAnimatedMeshComponent.h"
#include "../Common/AActor.h"

namespace Engine {
class AAnimatedMesh final : public AActor {
   private:
    UAnimatedMeshComponent* m_Mesh;

   public:
    AAnimatedMesh(UWorld* world, entt::entity handle, const std::string& name,
                  const std::string path)
        : AActor(world, handle, name) {
        m_Mesh = m_World->RegisterComponents<UAnimatedMeshComponent>(
            m_EntityHandle, path);
    }
};
}  // namespace Engine