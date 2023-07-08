#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/GameFramework/StaticMesh/UStaticMeshComponent.h>

namespace Engine
{
    class UPawnComponent : public UComponent
    {

    private:
        UStaticMeshComponent m_PawnStaticMesh;

    public:
        UPawnComponent();
        ~UPawnComponent();

    public:
        void Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform);
    };
} // namespace Engine