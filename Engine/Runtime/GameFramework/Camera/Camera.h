#pragma once

#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Core/Events/ApplicationEvent.h>
#include <Engine/Runtime/Core/Events/MouseEvent.h>

#include <Engine/Runtime/GameFramework/Camera/Frustum.h>

namespace Engine
{
    class Camera
    {
    public:
        Camera();
        virtual void              SetViewPort(uint32_t width, uint32_t height) = 0;
        virtual const std::string GetCameraType() const                        = 0;

        glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }

        void OnEvent(Event& event);
        bool OnWindowResizeEvent(WindowResizeEvent& event);

        virtual void    RecalculateProjectionMatrix()                                                 = 0;
        virtual Frustum CreateFrustumFromCamera(const UTransformComponent& transform) = 0;

    public:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectMatrix;
    };

    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float nearClip, float farClip);
        virtual void SetViewPort(uint32_t width, uint32_t height) override;
        void         RecalculateProjectionMatrix() override;

    public:
        virtual const std::string GetCameraType() const override { return "OrthographicCamera"; }

    public:
        const std::string m_CameraType = "OrthographicCamera";

        float m_Left;
        float m_Right;
        float m_Bottom;
        float m_Top;
        float m_NearClip;
        float m_FarClip;
    };

    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);
        virtual void SetViewPort(uint32_t width, uint32_t height) override;
        void         RecalculateProjectionMatrix() override;

    public:
        virtual const std::string GetCameraType() const { return "PerspectiveCamera"; }
        float&                    GetFovRef() { return m_Fov; }
        float&                    GetAspectRatioRef() { return m_AspectRatio; }
        float&                    GetNearClipRef() { return m_NearClip; }
        float&                    GetFarClipRef() { return m_FarClip; }
        Frustum CreateFrustumFromCamera(const UTransformComponent& transform) override;

    public:
        float m_Fov;
        float m_AspectRatio;
        float m_NearClip;
        float m_FarClip;
    };
} // namespace Engine