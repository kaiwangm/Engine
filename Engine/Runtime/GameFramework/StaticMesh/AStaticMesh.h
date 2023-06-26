#pragma once
#include <Engine/Runtime/GameFramework/StaticMesh/UStaticMeshComponent.h>
#include <Engine/Runtime/GameFramework/Common/AActor.h>
#include <Engine/Runtime/GameFramework/Material/MBasicPbr.h>
#include <Engine/Runtime/GameFramework/Material/MTriangleShader.h>

namespace Engine
{
    class AStaticMesh final : public AActor
    {
    private:
        UStaticMeshComponent* m_Mesh;
        Ref<MMaterial>        m_Material;

    public:
        AStaticMesh(UWorld* world, entt::entity handle, const std::string& name) : AActor(world, handle, name)
        {
            m_Mesh = m_World->RegisterComponents<UStaticMeshComponent>(m_EntityHandle);
            m_Mesh->SetOwner(this);

            m_Material = std::make_shared<MBasicPbr>(name);
        }
        AStaticMesh(UWorld*            world,
                    entt::entity       handle,
                    const std::string& name,
                    const std::string& path,
                    const std::string& materialMode,
                    const std::string& materialName,
                    const std::string& materialFolderName = "") :
            AActor(world, handle, name)
        {
            m_Mesh = m_World->RegisterComponents<UStaticMeshComponent>(m_EntityHandle, path);
            m_Mesh->SetOwner(this);

            if (materialMode == "BasicPbr")
            {
                m_Material = std::make_shared<MBasicPbr>(materialName);
            }
            else if (materialMode == "Texture")
            {
                m_Material = std::make_shared<MBasicPbr>(materialName, materialFolderName);
            }
            else if (materialMode == "TriangleShader")
            {
                m_Material = std::make_shared<MTriangleShader>(materialName);
            }
        }

        MMaterial* GetMaterial() { return m_Material.get(); }
    };
}; // namespace Engine