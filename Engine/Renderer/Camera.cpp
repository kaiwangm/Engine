#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {
Camera::Camera() : m_ProjectionMatrix(1.0f), m_ViewMatrix(1.0f) {
    m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation),
                                   glm::vec3(0.0f, 0.0f, 1.0f));

    m_ViewMatrix = glm::inverse(transform * rotate);
    m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                       float top) {
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio,
                                     float nearClip, float farClip) {
    m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
}  // namespace Engine