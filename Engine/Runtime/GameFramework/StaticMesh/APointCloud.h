#pragma once
#include <Engine/Runtime/GameFramework/StaticMesh/UPointCloudComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>

namespace Engine
{
    class APointCloud final : public AActor
    {
    private:
        UPointCloudComponent* m_Mesh;

    public:
        APointCloud(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_Mesh = m_World->RegisterComponents<UPointCloudComponent>(m_EntityHandle);
            m_Mesh->SetOwner(this);
        }
        APointCloud(UWorld*            world,
                    entt::entity       handle,
                    const std::string& name,
                    const std::string& path
                    ) :
            AActor(world, handle, name)
        {
            m_Mesh = m_World->RegisterComponents<UPointCloudComponent>(m_EntityHandle, path);
            m_Mesh->SetOwner(this);
        }
    };
}; // namespace Engine