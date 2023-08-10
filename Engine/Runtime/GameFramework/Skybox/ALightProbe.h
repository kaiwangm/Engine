#pragma once
#include <Engine/Runtime/GameFramework/Skybox/ULightProbeComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>

namespace Engine
{
    class ALightProbe final : public AActor
    {
    private:
        ULightProbeComponent* m_LightProbe;

    public:
        ALightProbe(UWorld* world, entt::entity handle, const std::string& name) :
            AActor(world, handle, name)
        {
            m_LightProbe = m_World->RegisterComponents<ULightProbeComponent>(m_EntityHandle);
            m_LightProbe->SetOwner(this);
        }

        ULightProbeComponent& GetLightProbeComponent() { return *m_LightProbe; }
    };
}; // namespace Engine