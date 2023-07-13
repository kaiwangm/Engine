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
        glm::quat m_nowPawnRight =
            glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        glm::quat m_nowCameraFoward = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
        glm::quat m_nowCameraRight =
            glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        glm::quat m_nowDesiredFoward = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
        glm::quat m_nowDesiredRight =
            glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        // SkinnedMesh
        glm::vec3 m_nowMeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::quat m_nowMeshFoward = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
        glm::quat m_nowMeshRight =
            glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

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
        glm::quat& GetNowDesiredFowardRef() { return m_nowDesiredFoward; }
        glm::quat& GetNowDesiredRightRef() { return m_nowDesiredRight; }
        glm::vec3& GetNowMeshPositionRef() { return m_nowMeshPosition; }
        glm::quat& GetNowMeshFowardRef() { return m_nowMeshFoward; }
        glm::quat& GetNowMeshRightRef() { return m_nowMeshRight; }

        void SetNowPosition(const glm::vec3& position) { m_nowPosition = position; }
        void SetNowPawnFoward(const glm::quat& foward) { m_nowPawnFoward = foward; }
        void SetNowPawnRight(const glm::quat& right) { m_nowPawnRight = right; }
        void SetNowCameraFoward(const glm::quat& foward) { m_nowCameraFoward = foward; }
        void SetNowCameraRight(const glm::quat& right) { m_nowCameraRight = right; }
        void SetNowDesiredFoward(const glm::quat& foward) { m_nowDesiredFoward = foward; }
        void SetNowDesiredRight(const glm::quat& right) { m_nowDesiredRight = right; }
        void SetNowMeshPosition(const glm::vec3& position) { m_nowMeshPosition = position; }
        void SetNowMeshFoward(const glm::quat& foward) { m_nowMeshFoward = foward; }
        void SetNowMeshRight(const glm::quat& right) { m_nowMeshRight = right; }
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