#pragma once
#include "UCameraComponent.h"

namespace Engine {
class ACamera final : public AActor {
   private:
    UCameraComponent* m_Camera;

   public:
    ACamera(UWorld* world, entt::entity handle, const std::string& name,
            const Ref<Camera> camera)
        : AActor(world, handle, name) {
        m_Camera = m_World->RegisterComponents<UCameraComponent>(m_EntityHandle,
                                                                 camera);
    }
};
};  // namespace Engine