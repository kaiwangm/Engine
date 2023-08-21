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
        glm::vec3 m_Velocity;

        float m_Time;

        static TrajectoryPoint mix(const TrajectoryPoint& a, const TrajectoryPoint& b, float mixFactor)
        {
            TrajectoryPoint result;

            result.m_Position    = glm::mix(a.m_Position, b.m_Position, mixFactor);
            result.m_Orientation = glm::slerp(a.m_Orientation, b.m_Orientation, mixFactor);
            result.m_Velocity    = glm::mix(a.m_Velocity, b.m_Velocity, mixFactor);

            result.m_Time = 0.0f;

            return result;
        }
    };

    struct TrajectoryPointArray
    {
        float                       m_SumTime;
        std::deque<TrajectoryPoint> m_TrajectoryPoints;
        std::deque<float>           m_TrajectoryPointsTime;

        TrajectoryPointArray() : m_SumTime(0.0f) {}

        void push_back(TrajectoryPoint point)
        {
            m_TrajectoryPoints.push_back(point);
            m_SumTime += point.m_Time;
            m_TrajectoryPointsTime.push_back(m_SumTime);
        }

        void push_front(TrajectoryPoint point)
        {
            m_TrajectoryPoints.push_front(point);
            m_SumTime = 0.0f;
            m_TrajectoryPointsTime.clear();
            for (auto& point : m_TrajectoryPoints)
            {
                m_SumTime += point.m_Time;
                m_TrajectoryPointsTime.push_back(m_SumTime);
            }
        }

        TrajectoryPoint getTrajectoryPoint(float time)
        {
            if (time < 0.0f)
            {
                return m_TrajectoryPoints.front();
            }

            if (time > m_SumTime)
            {
                return m_TrajectoryPoints.back();
            }

            auto it    = std::lower_bound(m_TrajectoryPointsTime.begin(), m_TrajectoryPointsTime.end(), time);
            int  index = it - m_TrajectoryPointsTime.begin();

            if (index >= m_TrajectoryPoints.size() - 1)
            {
                return m_TrajectoryPoints.back();
            }

            float mixFactor = 1.0 - (m_TrajectoryPointsTime[index] - time) / m_TrajectoryPoints[index].m_Time;

            glm::vec3 velocity = (m_TrajectoryPoints[index + 1].m_Position - m_TrajectoryPoints[index].m_Position) /
                                 m_TrajectoryPoints[index].m_Time;
            TrajectoryPoint result = TrajectoryPoint::mix(
                m_TrajectoryPoints[index], m_TrajectoryPoints[index + 1], glm::min(mixFactor, 1.0f));
            result.m_Velocity = velocity;

            return result;
        }

        size_t size() { return m_TrajectoryPoints.size(); }

        void clear()
        {
            m_SumTime = 0.0f;
            m_TrajectoryPoints.clear();
            m_TrajectoryPointsTime.clear();
        }

        void pop_back()
        {
            if (m_TrajectoryPoints.size() == 0)
            {
                return;
            }

            m_TrajectoryPoints.pop_back();
            m_TrajectoryPointsTime.clear();

            m_SumTime = 0.0f;
            for (auto& point : m_TrajectoryPoints)
            {
                m_SumTime += point.m_Time;
                m_TrajectoryPointsTime.push_back(m_SumTime);
            }
        }

        void pop_front()
        {
            if (m_TrajectoryPoints.size() == 0)
            {
                return;
            }

            m_TrajectoryPoints.pop_front();
            m_TrajectoryPointsTime.clear();

            m_SumTime = 0.0f;
            for (auto& point : m_TrajectoryPoints)
            {
                m_SumTime += point.m_Time;
                m_TrajectoryPointsTime.push_back(m_SumTime);
            }
        }
    };

    template<int dimDeepPhase>
    struct ManiflowPoint
    {
        std::array<float, dimDeepPhase> m_Maniflow;
        float                           m_Time;

        static ManiflowPoint mix(const ManiflowPoint& a, const ManiflowPoint& b, float mixFactor)
        {
            ManiflowPoint result;

            for (int i = 0; i < dimDeepPhase; i++)
            {
                result.m_Maniflow[i] = glm::mix(a.m_Maniflow[i], b.m_Maniflow[i], mixFactor);
            }

            result.m_Time = 0.0f;

            return result;
        }
    };

    template<int dimDeepPhase>
    struct ManiflowArray
    {
        float                                   m_SumTime;
        std::deque<ManiflowPoint<dimDeepPhase>> m_Maniflows;
        std::deque<float>                       m_ManiflowsTime;

        ManiflowArray() : m_SumTime(0.0f) {}

        void push_back(const ManiflowPoint<dimDeepPhase>& point)
        {
            m_Maniflows.push_back(point);
            m_SumTime += point.m_Time;
            m_ManiflowsTime.push_back(m_SumTime);
        }

        void push_front(const ManiflowPoint<dimDeepPhase>& point)
        {
            m_Maniflows.push_front(point);
            m_SumTime = 0.0f;
            m_ManiflowsTime.clear();
            for (auto& point : m_Maniflows)
            {
                m_SumTime += point.m_Time;
                m_ManiflowsTime.push_back(m_SumTime);
            }
        }

        ManiflowPoint<dimDeepPhase> getManiflowPoint(float time)
        {
            if (time < 0.0f)
            {
                return m_Maniflows.front();
            }

            if (time > m_SumTime)
            {
                return m_Maniflows.back();
            }

            auto it    = std::lower_bound(m_ManiflowsTime.begin(), m_ManiflowsTime.end(), time);
            int  index = it - m_ManiflowsTime.begin();

            if (index >= m_Maniflows.size() - 1)
            {
                return m_Maniflows.back();
            }

            float mixFactor = 1.0 - (m_ManiflowsTime[index] - time) / m_Maniflows[index].m_Time;

            ManiflowPoint<dimDeepPhase> result =
                ManiflowPoint<dimDeepPhase>::mix(m_Maniflows[index], m_Maniflows[index + 1], glm::min(mixFactor, 1.0f));

            return result;
        }

        size_t size() { return m_Maniflows.size(); }

        void clear()
        {
            m_SumTime = 0.0f;
            m_Maniflows.clear();
            m_ManiflowsTime.clear();
        }

        void pop_back()
        {
            if (m_Maniflows.size() == 0)
            {
                return;
            }

            m_Maniflows.pop_back();
            m_ManiflowsTime.clear();

            m_SumTime = 0.0f;
            for (auto& point : m_Maniflows)
            {
                m_SumTime += point.m_Time;
                m_ManiflowsTime.push_back(m_SumTime);
            }
        }

        void pop_front()
        {
            if (m_Maniflows.size() == 0)
            {
                return;
            }

            m_Maniflows.pop_front();
            m_ManiflowsTime.clear();

            m_SumTime = 0.0f;
            for (auto& point : m_Maniflows)
            {
                m_SumTime += point.m_Time;
                m_ManiflowsTime.push_back(m_SumTime);
            }
        }
    };

    struct KnnResult
    {
        int   index;
        float nowRatio;
        float loss;
    };

    class UTrajectoryComponent : public UComponent
    {
    private:
        UStaticMeshComponent m_TrajectoryStaticMesh;

    private:
        TrajectoryPointArray m_TrajectoryPointArray_Back;
        TrajectoryPointArray m_TrajectoryPointArray_Forward;
        KnnResult            m_SearchResult {
            0,
            0.0f,
            0.0f,
        };

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
        glm::quat m_nowMeshFoward   = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
        glm::quat m_nowMeshRight =
            glm::rotate(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        int   m_TrajectorySampleNum  = 10;
        float m_TrajectorySampleStep = 0.15f;
        float m_DesiredMoveSpeed     = 0.0f;

        float m_DesiredNowMoveSpeed = 0.0f;

        glm::vec3 m_NowMeshVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

    public:
        UTrajectoryComponent();
        ~UTrajectoryComponent();

    public:
        void TickLogic(float deltaTime, const glm::mat4& cameraTransform);
        void Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform);

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

        void SetTrajectorySampleNum(int num) { m_TrajectorySampleNum = num; }
        void SetTrajectorySampleStep(float step) { m_TrajectorySampleStep = step; }
        void SetDesiredMoveSpeed(float speed) { m_DesiredMoveSpeed = speed; }

        KnnResult  GetSearchResult() { return m_SearchResult; }
        glm::vec3& GetNowMeshVelocityRef() { return m_NowMeshVelocity; }
    };

    class UPawnComponent : public UComponent
    {
    private:
        UStaticMeshComponent m_PawnStaticMesh;

    public:
        float m_CameraDistance  = 5.0f;
        float m_CameraLongitude = glm::radians(90.0f);
        float m_CameraLatitude  = glm::radians(60.0f);

        glm::vec3 m_CameraLookAt = glm::vec3(0.0f, 0.8f, 0.0f);

        float m_PawnRunSpeed  = 3.00f;
        float m_PawnWalkSpeed = 1.18f;

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

        float& GetMouseSensitivityXRef() { return m_MouseSensitivityX; }
        float& GetMouseSensitivityYRef() { return m_MouseSensitivityY; }
    };
} // namespace Engine