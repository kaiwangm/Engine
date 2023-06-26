#pragma once
#include <Engine/Runtime/GameFramework/Animation/UAnimatedMeshComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>

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
        m_Mesh->SetOwner(this);
    }
};
}  // namespace Engine