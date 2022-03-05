#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Engine {
class Camera {
   public:
    Camera();
    virtual void SetPosition(const glm::vec3& position) {
        m_Position = position;
        RecalculateViewMatrix();
    }
    virtual void SetRotation(const float& rotation) {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }
    virtual void SetViewPort(uint32_t width, uint32_t height) = 0;
    virtual void SetCameraTranslationSpeed(float speed) {
        m_CameraTranslationSpeed = speed;
    }
    virtual void SetCameraRotationSpeed(float speed) {
        m_CameraRotationSpeed = speed;
    }

    const glm::vec3& GetPosition() const { return m_Position; }
    const float& GetRotation() const { return m_Rotation; }
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectMatrix() const {
        return m_ViewProjectMatrix;
    }

    void OnUpdate(float timeStep);
    void OnEvent(Event& event);
    bool OnWindowResizeEvent(WindowResizeEvent& event);

   private:
    void RecalculateViewMatrix();
    virtual void RecalculateProjectionMatrix() = 0;

   protected:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectMatrix;

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Rotation = 0.0f;

   private:
    float m_CameraTranslationSpeed = 3.0;
    float m_CameraRotationSpeed = 3.0;
};

class OrthographicCamera : public Camera {
   public:
    OrthographicCamera(float left, float right, float bottom, float top,
                       float nearClip, float farClip);
    virtual void SetViewPort(uint32_t width, uint32_t height) override;

   private:
    float m_Left;
    float m_Right;
    float m_Bottom;
    float m_Top;
    float m_NearClip;
    float m_FarClip;
    void RecalculateProjectionMatrix() override;
};

class PerspectiveCamera : public Camera {
   public:
    PerspectiveCamera(float fov, float aspectRatio, float nearClip,
                      float farClip);
    virtual void SetViewPort(uint32_t width, uint32_t height) override;

   private:
    float m_Fov;
    float m_AspectRatio;
    float m_NearClip;
    float m_FarClip;
    void RecalculateProjectionMatrix() override;
};

}  // namespace Engine