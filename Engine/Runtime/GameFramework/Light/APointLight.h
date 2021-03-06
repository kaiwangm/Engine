#pragma once
#include "UPointLightComponent.h"
#include "../Common/AActor.h"

namespace Engine
{
    class APointLight final : public AActor
    {
    private:
        UPointLightComponent* m_Light;

    public:
        APointLight(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_Light = m_World->RegisterComponents<UPointLightComponent>(m_EntityHandle);
            m_Light->SetOwner(this);
        }
    };
} // namespace Engine