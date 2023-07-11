#pragma once
#include <Engine/Runtime/GameFramework/Animation/USkinnedMeshComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>
#include <Engine/Runtime/GameFramework/Material/MBasicPbr.h>

namespace Engine
{
    class ASkinnedMesh final : public AActor
    {
    private:
        USkinnedMeshComponent* m_SkinnedMesh;
        Ref<MMaterial>        m_Material;

    public:
        ASkinnedMesh(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_SkinnedMesh = m_World->RegisterComponents<USkinnedMeshComponent>(m_EntityHandle);
            m_SkinnedMesh->SetOwner(this);

            m_Material = std::make_shared<MBasicPbr>(name);
        }

        ASkinnedMesh(UWorld*            world,
                     entt::entity       handle,
                     const std::string& name,
                     const std::string& skeletonPath,
                     const std::string& animationPath,
                     const std::string& meshPath) :
            AActor(world, handle, name)
        {
            m_SkinnedMesh = m_World->RegisterComponents<USkinnedMeshComponent>(
                m_EntityHandle, skeletonPath, animationPath, meshPath);
            m_SkinnedMesh->SetOwner(this);

            m_Material = std::make_shared<MBasicPbr>(name);

            MBasicPbr* material = static_cast<MBasicPbr*>(m_Material.get());

            material->SetAlbedo(glm::vec3(0.9f, 0.9f, 0.9f));
            material->SetMetallic(0.03f);
            material->SetRoughness(0.96f);
        }

        MMaterial* GetMaterial() { return m_Material.get(); }
    };
} // namespace Engine