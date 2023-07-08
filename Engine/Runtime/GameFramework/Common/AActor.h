#pragma once

#include <entt/entt.hpp>
#include <Engine/Runtime/GameFramework/Common/UObject.h>
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/GameFramework/Common/UWorld.h>

namespace Engine
{
    class AActor : public UObject
    {
    protected:
        entt::entity m_EntityHandle {entt::null};
        UWorld*      m_World;

    private:
        UTagComponent*       m_ActorName;
        UTransformComponent* m_Transform;

    public:
        AActor(UWorld* world, entt::entity handle, const std::string& name) : m_World(world), m_EntityHandle(handle)
        {
            m_ActorName = m_World->RegisterComponents<UTagComponent>(m_EntityHandle, name);
            m_ActorName->SetOwner(this);

            m_Transform = m_World->RegisterComponents<UTransformComponent>(
                m_EntityHandle, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
            m_Transform->SetOwner(this);
        }

        std::string          GetName() const { return m_ActorName->GetString(); }
        UTransformComponent& GetTransformComponent() { return *m_Transform; }

        bool m_IsControlled = false;

        bool  GetIsControlled() const { return m_IsControlled; }
        bool& GetIsControlledRef() { return m_IsControlled; }
        void  SetIsControlled(bool isControlled) { m_IsControlled = isControlled; }
    };
}; // namespace Engine