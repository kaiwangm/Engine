#pragma once
#include "../DataUtil/Animation.h"
#include "../Common/UComponent.h"

namespace Engine
{
    class UAnimatedMeshComponent : public UComponent
    {
    private:
        AnimatedMesh m_Model;

    public:
        UAnimatedMeshComponent(const std::string path) : m_Model(path) {}

        AnimatedMesh& GetModel() { return m_Model; }
    };
} // namespace Engine