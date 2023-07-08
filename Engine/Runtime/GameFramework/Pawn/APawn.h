#pragma once
#include <Engine/Runtime/GameFramework/Common/AActor.h>
#include <Engine/Runtime/GameFramework/Pawn/UPawnComponent.h>
#include <Engine/Runtime/GameFramework/Camera/UCameraComponent.h>

namespace Engine
{
    class APawn final : public AActor
    {
    private:
        UPawnComponent*   m_Pawn;
        UCameraComponent* m_Camera;

    public:
        APawn(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_Pawn = m_World->RegisterComponents<UPawnComponent>(m_EntityHandle);
            m_Pawn->SetOwner(this);

            auto& camera = std::make_shared<PerspectiveCamera>(60.0f, 1.778f, 0.1f, 800.0f);

            m_Camera = m_World->RegisterComponents<UCameraComponent>(m_EntityHandle, camera);
            m_Camera->SetOwner(this);

            m_Camera->SetPosition(glm::vec3 {0.0f, 3.0f, 5.0f});
            m_Camera->SetRotation(glm::vec3 {-0.35f, 0.0f, 0.0f});
        }
    };
} // namespace Engine