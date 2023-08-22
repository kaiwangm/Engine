#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/GameFramework/Camera/Camera.h>

namespace Engine
{
    class UCameraComponent : public UComponent
    {
    public:
        Ref<Camera> m_Camera;

        UCameraComponent(const Ref<Camera> camera) : m_Camera(camera)
        {
            m_Position = glm::vec3(0.0f);
            m_Rotation = glm::vec3(0.0f);
            m_Scale    = glm::vec3(1.0f);
        }

        Camera& GetCamera() { return *m_Camera; }

    public:
        bool m_IsViewportCamera = false;

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Rotation;
        glm::vec3 m_Scale;

    public:
        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

            return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
        }

        glm::vec3 GetPosition() const { return m_Position; }
        glm::vec3 GetRotation() const { return m_Rotation; }
        glm::vec3 GetScale() const { return m_Scale; }

        glm::vec3& GetPositionRef() { return m_Position; }
        glm::vec3& GetRotationRef() { return m_Rotation; }
        glm::vec3& GetScaleRef() { return m_Scale; }

        void SetPosition(const glm::vec3& position) { m_Position = position; }
        void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
        void SetScale(const glm::vec3& scale) { m_Scale = scale; }

        Frustum CreateFrustumFromCamera(const UTransformComponent& transform)
        {
            UTransformComponent cameraTransform;
    
            cameraTransform.SetPosition(m_Position);
            cameraTransform.SetRotation(m_Rotation);
            cameraTransform.SetScale(m_Scale);

            return m_Camera->CreateFrustumFromCamera(transform, cameraTransform);
        }

    public:
        bool  GetIsViewportCamera() const { return m_IsViewportCamera; }
        bool& GetIsViewportCameraRef() { return m_IsViewportCamera; }
        void  SetIsViewportCamera(bool isViewportCamera) { m_IsViewportCamera = isViewportCamera; }
    };
}; // namespace Engine
