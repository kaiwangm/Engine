#include <Engine/Runtime/GameFramework/Pawn/UPawnComponent.h>
#include <Engine/Runtime/GameFramework/Pawn/APawn.h>
#include <Engine/Runtime/Core/Input/Input.h>

namespace Engine
{
    UTrajectoryComponent::UTrajectoryComponent() : m_TrajectoryStaticMesh("Assert/Editor/Object/apawn/trajectory.obj")
    {}

    UTrajectoryComponent::~UTrajectoryComponent() {}

    void UTrajectoryComponent::TickLogic(float deltaTime, const glm::mat4& cameraTransform)
    {
        m_TrajecotryPoints.push_front({m_nowPosition, m_nowPawnFoward, deltaTime});

        if (m_TrajecotryPoints.size() > 100)
        {
            m_TrajecotryPoints.pop_back();
        }

        glm::vec3 target       = cameraTransform * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        float     rotateAngleX = glm::eulerAngles(glm::quat(cameraTransform)).x;
        float     rotateAngleY = glm::eulerAngles(glm::quat(cameraTransform)).y;

        if (target.y < 0.0f)
        {
            if (rotateAngleX > glm::radians(0.0f))
            {
                rotateAngleY = glm::radians(180.0f) - rotateAngleY;
            }
        }
        else
        {
            if (rotateAngleX < glm::radians(0.0f))
            {
                rotateAngleY = glm::radians(180.0f) - rotateAngleY;
            }
        }

        static bool initlized = false;

        m_nowCameraFoward =
            glm::quat(glm::rotate(glm::mat4(1.0f), rotateAngleY + glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

        m_nowCameraRight = glm::rotate(m_nowCameraFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        if (initlized == false)
        {
            m_nowDesiredFoward = m_nowCameraFoward;
            m_nowDesiredRight  = m_nowCameraRight;

            m_nowPawnFoward = m_nowCameraFoward;
            m_nowPawnRight  = m_nowCameraRight;

            initlized = true;
        }
    }

    TrajectoryPoint UTrajectoryComponent::GetTrajectoryPoint(float time)
    {
        float           sumTime = 0.0f;
        TrajectoryPoint result;

        if (m_TrajecotryPoints.size() <= 1)
        {
            UTransformComponent& transformComponent = static_cast<AActor*>(m_Owner)->GetTransformComponent();

            result.m_Position    = transformComponent.GetPosition();
            result.m_Orientation = transformComponent.GetRotation();
            result.m_Time        = m_SampleStep;

            return result;
        }

        for (int i = 0; i < m_TrajecotryPoints.size() - 1; ++i)
        {
            sumTime += m_TrajecotryPoints[i].m_Time;

            if (sumTime >= time)
            {
                glm::vec3 position_a = m_TrajecotryPoints[i].m_Position;
                glm::vec3 position_b = m_TrajecotryPoints[i + 1].m_Position;

                glm::quat orientation_a = m_TrajecotryPoints[i].m_Orientation;
                glm::quat orientation_b = m_TrajecotryPoints[i + 1].m_Orientation;

                float mixFactor = 1.0 - (sumTime - time) / m_TrajecotryPoints[i].m_Time;

                result.m_Position            = glm::mix(position_a, position_b, mixFactor);
                glm::quat orientation_a_quat = glm::slerp(orientation_a, orientation_b, mixFactor);
                result.m_Orientation         = glm::eulerAngles(orientation_a_quat);
                result.m_Time                = m_SampleStep;

                break;
            }
        }

        if (sumTime < time)
        {
            result.m_Position    = m_TrajecotryPoints.back().m_Position;
            result.m_Orientation = m_TrajecotryPoints.back().m_Orientation;
            result.m_Time        = m_SampleStep;
        }

        return result;
    }

    void UTrajectoryComponent::Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform)
    {
        auto& vertexArray = m_TrajectoryStaticMesh.GetStaticMesh().m_Meshes[0].m_VertexArray;

        shader->Bind();

        // draw axis
        for (int i = 1; i < m_SampleNum; ++i)
        {
            float           nowTime         = i * m_SampleStep;
            TrajectoryPoint trajectoryPoint = GetTrajectoryPoint(nowTime);

            glm::mat4 translation = glm::translate(glm::mat4(1.0f), trajectoryPoint.m_Position);
            glm::mat4 rotation    = glm::toMat4(glm::quat(trajectoryPoint.m_Orientation));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 1.0f, 1.0f));

            glm::mat4 rotationX =
                rotation * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 rotationY =
                rotation * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 rotationZ =
                rotation * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            shader->SetMat4("u_ViewProjection", vpMat);

            // axis x
            {
                glm::mat4 localTransform = translation * rotationX * scale;
                shader->SetMat4("u_Transform", localTransform);
                shader->SetFloat3("u_Color", glm::vec3(0.5f, 0.0f, 0.0f));

                vertexArray->Bind();
                RenderCommand::DrawIndexed(vertexArray);
                vertexArray->UnBind();
            }

            // axis y
            {
                glm::mat4 localTransform = translation * rotationY * scale;
                shader->SetMat4("u_Transform", localTransform);
                shader->SetFloat3("u_Color", glm::vec3(0.0f, 0.5f, 0.0f));

                vertexArray->Bind();
                RenderCommand::DrawIndexed(vertexArray);
                vertexArray->UnBind();
            }

            // axis z
            {
                glm::mat4 localTransform = translation * rotationZ * scale;
                shader->SetMat4("u_Transform", localTransform);
                shader->SetFloat3("u_Color", glm::vec3(0.0f, 0.0f, 0.5f));

                vertexArray->Bind();
                RenderCommand::DrawIndexed(vertexArray);
                vertexArray->UnBind();
            }
        }

        // draw nowPawnFoward
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowPosition);
            glm::mat4 rotation    = glm::toMat4(glm::quat(m_nowPawnFoward));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * rotation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.0f, 0.0f));

            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
            vertexArray->UnBind();
        }

        // draw nowCameraFoward
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowPosition);
            glm::mat4 rotation    = glm::toMat4(glm::quat(m_nowCameraFoward));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * rotation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.0f, 0.0f, 0.9f));

            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
            vertexArray->UnBind();
        }

        // draw nowDesiredFoward
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowPosition);
            glm::mat4 rotation    = glm::toMat4(glm::quat(m_nowDesiredFoward));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * rotation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.9f, 0.9f));

            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
            vertexArray->UnBind();
        }

        // draw trajectory
        for (int i = 0; i < m_SampleNum; ++i)
        {
            float nowTime  = i * m_SampleStep;
            float nextTime = (i + 1) * m_SampleStep;

            TrajectoryPoint trajectoryPoint     = GetTrajectoryPoint(nowTime);
            TrajectoryPoint nextTrajectoryPoint = GetTrajectoryPoint(nextTime);

            glm::vec3 vecDir   = nextTrajectoryPoint.m_Position - trajectoryPoint.m_Position;
            float     distance = glm::length(vecDir);
            vecDir             = glm::normalize(vecDir);

            glm::mat4 translation = glm::translate(glm::mat4(1.0f), trajectoryPoint.m_Position);
            glm::mat4 rotation    = glm::toMat4(glm::quat(glm::vec3(1.0f, 0.0f, 0.0f), vecDir));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(distance, 1.0f, 1.0f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * rotation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.9f, 0.9f));

            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
            vertexArray->UnBind();
        }

        // draw nowPosition
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowPosition);
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.3f, 0.3f, 0.9f));

            auto& pawnStaticMeshvertexArray = static_cast<APawn*>(m_Owner)
                                                  ->GetPawnComponentRef()
                                                  .GetPawnStaticMeshRef()
                                                  .GetStaticMesh()
                                                  .m_Meshes[0]
                                                  .m_VertexArray;
            pawnStaticMeshvertexArray->Bind();
            RenderCommand::DrawIndexed(pawnStaticMeshvertexArray);

            pawnStaticMeshvertexArray->UnBind();
        }

        shader->UnBind();
    }

    UPawnComponent::UPawnComponent() : m_PawnStaticMesh("Assert/Editor/Object/apawn/apawn.obj") {}

    UPawnComponent::~UPawnComponent() {}

    void UPawnComponent::TickLogic(float deltaTime)
    {
        auto [currentX, currentY] = Input::GetMousePostion();

        static float lastX;
        static float lastY;

        float deltaX = currentX - lastX;
        float deltaY = currentY - lastY;

        lastX = currentX;
        lastY = currentY;

        AActor* actor = static_cast<AActor*>(m_Owner);
        APawn*  aPawn = static_cast<APawn*>(actor);

        UTransformComponent&  transformComponent  = actor->GetTransformComponent();
        UTrajectoryComponent& trajectoryComponent = aPawn->GetTrajectoryComponentRef();

        glm::vec3 worldPosition = transformComponent.GetPosition();

        glm::vec3& pawnPosition = trajectoryComponent.GetNowPositionRef();
        glm::quat& pawnFoward   = trajectoryComponent.GetNowPawnFowardRef();
        glm::quat& pawnRight    = trajectoryComponent.GetNowPawnRightRef();

        glm::quat& cameraFoward = trajectoryComponent.GetNowCameraFowardRef();
        glm::quat& cameraRight  = trajectoryComponent.GetNowCameraRightRef();

        glm::quat& desiredFoward = trajectoryComponent.GetNowDesiredFowardRef();
        glm::quat& desiredRight  = trajectoryComponent.GetNowDesiredRightRef();

        const glm::quat& nowPawnFoward = trajectoryComponent.GetNowPawnFowardRef();
        const glm::quat& nowPawnRight  = trajectoryComponent.GetNowPawnRightRef();

        glm::vec3 movement = glm::vec3(0.0f);
        glm::vec3 forward  = glm::vec3(glm::mat4(nowPawnFoward) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        glm::vec3 right    = glm::vec3(glm::mat4(nowPawnRight) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

        bool isMoved = false;
        if (actor->GetIsControlled())
        {
            if (Input::IsKeyPressed(GLFW_KEY_W))
            {
                const glm::quat direction = glm::rotate(cameraFoward, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                desiredFoward = glm::slerp(desiredFoward, direction, 7.0f * deltaTime);
                isMoved       = true;
            }
            if (Input::IsKeyPressed(GLFW_KEY_S))
            {
                const glm::quat direction = glm::rotate(cameraFoward, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                desiredFoward = glm::slerp(desiredFoward, direction, 7.0f * deltaTime);
                isMoved       = true;
            }
            if (Input::IsKeyPressed(GLFW_KEY_A))
            {
                const glm::quat direction = glm::rotate(cameraFoward, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                desiredFoward = glm::slerp(desiredFoward, direction, 7.0f * deltaTime);
                isMoved       = true;
            }
            if (Input::IsKeyPressed(GLFW_KEY_D))
            {
                const glm::quat direction = glm::rotate(cameraFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                desiredFoward = glm::slerp(desiredFoward, direction, 7.0f * deltaTime);
                isMoved       = true;
            }

            if (isMoved == true)
            {
                movement += forward;
                movement = glm::normalize(movement);
            }

            if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
            {
                Input::HideCursor();
                m_CameraLongitude += deltaX * m_MouseSensitivityX * deltaTime;
                m_CameraLatitude -= deltaY * m_MouseSensitivityY * deltaTime;
            }

            auto newPosition = transformComponent.GetPosition() + movement * m_PawnMoveSpeed * deltaTime;
            transformComponent.SetPosition(newPosition);
        }

        // Update trajectory
        if (isMoved == false)
        {
            desiredFoward = glm::slerp(desiredFoward, pawnFoward, 7.0f * deltaTime);
        }
        desiredRight  = glm::rotate(desiredFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        pawnPosition = glm::mix(pawnPosition, worldPosition, 7.0f * deltaTime);

        pawnFoward = glm::slerp(pawnFoward, desiredFoward, 7.0f * deltaTime);
        pawnRight  = glm::rotate(pawnFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        // Update camera
        UCameraComponent& cameraComponent = static_cast<APawn*>(m_Owner)->GetCameraComponentRef();

        glm::vec3 position;

        position.x = m_CameraDistance * sin(m_CameraLatitude) * cos(m_CameraLongitude);
        position.y = m_CameraDistance * cos(m_CameraLatitude);
        position.z = m_CameraDistance * sin(m_CameraLatitude) * sin(m_CameraLongitude);

        position = m_CameraLookAt + position;

        glm::vec3 orientation = glm::eulerAngles(
            glm::quat(glm::inverse(glm::lookAt(position, m_CameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f)))));

        cameraComponent.SetPosition(position);
        cameraComponent.SetRotation(orientation);
    }

    void UPawnComponent::Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform)
    {
        shader->Bind();

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));

        shader->SetMat4("u_ViewProjection", vpMat);
        shader->SetMat4("u_Transform", transform * scale);
        shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.3f, 0.3f));

        auto& vertexArray = m_PawnStaticMesh.GetStaticMesh().m_Meshes[0].m_VertexArray;
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);

        vertexArray->UnBind();

        shader->UnBind();
    }

} // namespace Engine