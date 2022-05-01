#pragma once
#include "USkyboxComponent.h"
#include "../Common/AActor.h"

namespace Engine
{
    class ASkybox final : public AActor
    {
    private:
        USkyboxComponent* m_Skybox;

    public:
        ASkybox(UWorld* world, entt::entity handle, const std::string& name, const std::string path) :
            AActor(world, handle, name)
        {
            m_Skybox = m_World->RegisterComponents<USkyboxComponent>(m_EntityHandle, path);
            m_Skybox->SetOwner(this);
        }

        USkyboxComponent& GetSkyboxComponent() { return *m_Skybox; }
    };
}; // namespace Engine