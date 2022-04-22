#pragma once
#include "StaticMesh.h"
#include "../Common/UComponent.h"

namespace Engine
{
    class UStaticMeshComponent final : public UComponent
    {
    private:
        StaticMesh m_StaticMesh;

    public:
        UStaticMeshComponent() : m_StaticMesh() {}
        UStaticMeshComponent(const std::string path) : m_StaticMesh(path) {}

        StaticMesh& GetStaticMesh() { return m_StaticMesh; }
    };
}; // namespace Engine