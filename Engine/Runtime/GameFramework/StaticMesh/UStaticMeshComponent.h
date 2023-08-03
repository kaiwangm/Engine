#pragma once
#include <Engine/Runtime/GameFramework/DataUtil/StaticMesh.h>
#include <Engine/Runtime/GameFramework/Common/UComponent.h>

namespace Engine
{
    class UStaticMeshComponent final : public UComponent
    {
    private:
        StaticMesh m_StaticMesh;

    public:
        UStaticMeshComponent() : m_StaticMesh() {}
        UStaticMeshComponent(const std::string path, const bool batching = false) : m_StaticMesh(path, batching) {}

        StaticMesh& GetStaticMesh() { return m_StaticMesh; }
    };
}; // namespace Engine