#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/GameFramework/StaticMesh/UStaticMeshComponent.h>

#include <deque>

namespace Engine
{
    struct TrajectoryPoint
    {
        glm::vec3 m_Position;
        glm::quat m_Orientation;
        float     m_Time;
    };

    class UTrajectoryComponent : public UComponent
    {
    private:
        UStaticMeshComponent        m_TrajectoryStaticMesh;
        std::deque<TrajectoryPoint> m_TrajecotryPoints;

    public:
        glm::vec3 m_nowPosition = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::quat m_nowPawnFoward = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
        glm::quat m_nowPawnRight = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);

        glm::quat m_nowCameraFoward = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
        glm::quat m_nowCameraRight = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);

        float m_SampleStep = 0.1f;
        int   m_SampleNum  = 10;

    public:
        UTrajectoryComponent();
        ~UTrajectoryComponent();

    public:
        TrajectoryPoint GetTrajectoryPoint(float time);
        void            TickLogic(float deltaTime, const glm::mat4& cameraTransform);
        void            Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform);

        glm::vec3& GetNowPositionRef() { return m_nowPosition; }
        glm::quat& GetNowPawnFowardRef() { return m_nowPawnFoward; }
        glm::quat& GetNowPawnRightRef() { return m_nowPawnRight; }
        glm::quat& GetNowCameraFowardRef() { return m_nowCameraFoward; }
        glm::quat& GetNowCameraRightRef() { return m_nowCameraRight; }

        void SetNowPosition(const glm::vec3& position) { m_nowPosition = position; }
        void SetNowPawnFoward(const glm::quat& foward) { m_nowPawnFoward = foward; }
        void SetNowPawnRight(const glm::quat& right) { m_nowPawnRight = right; }
        void SetNowCameraFoward(const glm::quat& foward) { m_nowCameraFoward = foward; }
        void SetNowCameraRight(const glm::quat& right) { m_nowCameraRight = right; }
    };

    class UPawnComponent : public UComponent
    {
    private:
        UStaticMeshComponent m_PawnStaticMesh;

    public:
        float m_CameraDistance  = 7.0f;
        float m_CameraLongitude = glm::radians(90.0f);
        float m_CameraLatitude  = glm::radians(60.0f);

        glm::vec3 m_CameraLookAt = glm::vec3(0.0f, 0.8f, 0.0f);

        float m_PawnMoveSpeed     = 7.0f;
        float m_MouseSensitivityX = 0.09f;
        float m_MouseSensitivityY = 0.09f;

    public:
        UPawnComponent();
        ~UPawnComponent();

    public:
        void TickLogic(float deltaTime);
        void Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform);

        UStaticMeshComponent& GetPawnStaticMeshRef() { return m_PawnStaticMesh; }

        float& GetCameraDistanceRef() { return m_CameraDistance; }
        float& GetCameraLongitudeRef() { return m_CameraLongitude; }
        float& GetCameraLatitudeRef() { return m_CameraLatitude; }

        glm::vec3& GetCameraLookAtRef() { return m_CameraLookAt; }

        float& GetPawnMoveSpeedRef() { return m_PawnMoveSpeed; }
        float& GetMouseSensitivityXRef() { return m_MouseSensitivityX; }
        float& GetMouseSensitivityYRef() { return m_MouseSensitivityY; }
    };
} // namespace Engine