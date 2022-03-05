#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Input.h"

namespace Engine {
Camera::Camera() : m_ProjectionMatrix(1.0f), m_ViewMatrix(1.0f) {
    m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::OnUpdate(float timeStep) {
    auto camera_pos = this->GetPosition();
    auto camera_rot = this->GetRotation();

    if (Input::IsKeyPressed(GLFW_KEY_A)) {
        camera_pos.x -= m_CameraTranslationSpeed * timeStep;
    }
    if (Input::IsKeyPressed(GLFW_KEY_D)) {
        camera_pos.x += m_CameraTranslationSpeed * timeStep;
    }
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        camera_pos.y -= m_CameraTranslationSpeed * timeStep;
    }
    if (Input::IsKeyPressed(GLFW_KEY_SPACE)) {
        camera_pos.y += m_CameraTranslationSpeed * timeStep;
    }
    if (Input::IsKeyPressed(GLFW_KEY_W)) {
        camera_pos.z -= m_CameraTranslationSpeed * timeStep;
    }
    if (Input::IsKeyPressed(GLFW_KEY_S)) {
        camera_pos.z += m_CameraTranslationSpeed * timeStep;
    }
    if (Input::IsKeyPressed(GLFW_KEY_E)) {
        camera_rot -= m_CameraRotationSpeed * timeStep;
    }
    if (Input::IsKeyPressed(GLFW_KEY_Q)) {
        camera_rot += m_CameraRotationSpeed * timeStep;
    }

    this->SetRotation(camera_rot);
    this->SetPosition(camera_pos);
}

void Camera::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(
        BIND_EVENT(Camera::OnWindowResizeEvent));
}

bool Camera::OnWindowResizeEvent(WindowResizeEvent& event) {
    SetViewPort(event.GetWidth(), event.GetHeight());
    return false;
}

void Camera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation),
                                   glm::vec3(0.0f, 0.0f, 1.0f));

    m_ViewMatrix = glm::inverse(transform * rotate);
}

void Camera::RecalculateViewProjectMatrix() {
    RecalculateViewMatrix();
    RecalculateProjectionMatrix();
    m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                       float top, float nearClip, float farClip)
    : m_Left(left),
      m_Right(right),
      m_Bottom(bottom),
      m_Top(top),
      m_NearClip(nearClip),
      m_FarClip(farClip) {
    m_ProjectionMatrix =
        glm::ortho(left, right, bottom, top, nearClip, farClip);
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::SetViewPort(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) return;

    float aspectRatio = (float)width / (float)height;
    m_Left = -aspectRatio;
    m_Right = aspectRatio;
    m_Bottom = -1.0f;
    m_Top = 1.0f;
}

void OrthographicCamera::RecalculateProjectionMatrix() {
    m_ProjectionMatrix =
        glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearClip, m_FarClip);
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio,
                                     float nearClip, float farClip)
    : m_Fov(fov),
      m_AspectRatio(aspectRatio),
      m_NearClip(nearClip),
      m_FarClip(farClip) {
    m_ProjectionMatrix =
        glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::SetViewPort(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) return;

    m_AspectRatio = (float)width / (float)height;
}

void PerspectiveCamera::RecalculateProjectionMatrix() {
    m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio,
                                          m_NearClip, m_FarClip);
}
}  // namespace Engine