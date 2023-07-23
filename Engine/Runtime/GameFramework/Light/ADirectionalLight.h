#pragma once
#include <Engine/Runtime/GameFramework/Light/UDirectionalLightComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>

namespace Engine
{
    class ADirectionalLight final : public AActor
    {
    private:
        UDirectionalLightComponent* m_Light;

    public:
        ADirectionalLight(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_Light = m_World->RegisterComponents<UDirectionalLightComponent>(m_EntityHandle);
            m_Light->SetOwner(this);
        }

        UDirectionalLightComponent& GetLightRef() { return *m_Light; }
    };
} // namespace Engine