#pragma once
#include <Engine/Runtime/GameFramework/Camera/UCameraComponent.h>

namespace Engine
{
    class ACamera final : public AActor
    {
    private:
        UCameraComponent* m_Camera;

    public:
        ACamera(UWorld* world, entt::entity handle, const std::string& name, const Ref<Camera> camera) :
            AActor(world, handle, name)
        {
            m_Camera = m_World->RegisterComponents<UCameraComponent>(m_EntityHandle, camera);
            m_Camera->SetOwner(this);
        }

        UCameraComponent& GetCameraComponent() { return *m_Camera; }

    public:
        bool m_IsViewportCamera = false;

    public:
        bool  GetIsViewportCamera() const { return m_IsViewportCamera; }
        bool& GetIsViewportCameraRef() { return m_IsViewportCamera; }
        void  SetIsViewportCamera(bool isViewportCamera) { m_IsViewportCamera = isViewportCamera; }
    };
}; // namespace Engine