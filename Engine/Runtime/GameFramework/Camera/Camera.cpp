#include <Engine/Runtime/GameFramework/Camera/Camera.h>
#include <Engine/Runtime/Core/Input/Input.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
    Camera::Camera() : m_ProjectionMatrix(1.0f), m_ViewMatrix(1.0f)
    {
        m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(Camera::OnWindowResizeEvent));
    }

    bool Camera::OnWindowResizeEvent(WindowResizeEvent& event)
    {
        SetViewPort(event.GetWidth(), event.GetHeight());
        return false;
    }

    OrthographicCamera::OrthographicCamera(float left,
                                           float right,
                                           float bottom,
                                           float top,
                                           float nearClip,
                                           float farClip) :
        m_Left(left),
        m_Right(right), m_Bottom(bottom), m_Top(top), m_NearClip(nearClip), m_FarClip(farClip)
    {
        m_ProjectionMatrix  = glm::ortho(left, right, bottom, top, nearClip, farClip);
        m_ViewMatrix        = glm::mat4(1.0f);
        m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetViewPort(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0)
            return;

        float aspectRatio = (float)width / (float)height;
        m_Left            = -aspectRatio;
        m_Right           = aspectRatio;
        m_Bottom          = -1.0f;
        m_Top             = 1.0f;
    }

    void OrthographicCamera::RecalculateProjectionMatrix()
    {
        m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearClip, m_FarClip);
    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip) :
        m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
    {
        m_ProjectionMatrix  = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        m_ViewMatrix        = glm::mat4(1.0f);
        m_ViewProjectMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void PerspectiveCamera::SetViewPort(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0)
            return;

        m_AspectRatio = (float)width / (float)height;
    }

    void PerspectiveCamera::RecalculateProjectionMatrix()
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
    }

    Frustum PerspectiveCamera::CreateFrustumFromCamera(const UTransformComponent& transform)
    {
        Frustum     frustum;
        const float halfVSide = m_FarClip * std::tanf(glm::radians(m_Fov) * .5f);
        const float halfHSide = halfVSide * m_AspectRatio;

        glm::vec3 position = transform.GetPosition();
        glm::vec3 rotation = transform.GetRotation();

        const glm::vec3 cameraFront = -glm::normalize(glm::rotate(glm::quat(rotation), glm::vec3(0.0f, 0.0f, 1.0f)));
        const glm::vec3 cameraRight = glm::normalize(glm::rotate(glm::quat(rotation), glm::vec3(1.0f, 0.0f, 0.0f)));
        const glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

        const glm::vec3 frontMultFar = m_FarClip * cameraFront;

        frustum.nearFace   = {position + m_NearClip * cameraFront, cameraFront};
        frustum.farFace    = {position + frontMultFar, -cameraFront};
        frustum.rightFace  = {position, glm::cross(frontMultFar - cameraRight * halfHSide, cameraUp)};
        frustum.leftFace   = {position, glm::cross(cameraUp, frontMultFar + cameraRight * halfHSide)};
        frustum.topFace    = {position, glm::cross(cameraRight, frontMultFar - cameraUp * halfVSide)};
        frustum.bottomFace = {position, glm::cross(frontMultFar + cameraUp * halfVSide, cameraRight)};

        return frustum;
    }
} // namespace Engine