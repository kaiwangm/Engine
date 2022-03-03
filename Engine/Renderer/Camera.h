#pragma once

#include "Core.h"

namespace Engine {
class Camera {
   public:
    Camera();
    void SetPosition(const glm::vec3& position) {
        m_Position = position;
        RecalculateViewMatrix();
    }
    void SetRotation(const float& rotation) {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    const glm::vec3& GetPosition() const { return m_Position; }
    const float& GetRotation() const { return m_Rotation; }
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectMatrix() const {
        return m_ViewProjectMatrix;
    }

   private:
    void RecalculateViewMatrix();

   protected:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectMatrix;

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Rotation = 0.0f;
};

class OrthographicCamera : public Camera {
   public:
    OrthographicCamera(float left, float right, float bottom, float top);
};

class PerspectiveCamera : public Camera {
   public:
    PerspectiveCamera(float fov, float aspectRatio, float nearClip,
                      float farClip);
};

}  // namespace Engine