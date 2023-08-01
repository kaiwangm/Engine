#include <Engine/Runtime/GameFramework/Pawn/UPawnComponent.h>
#include <Engine/Runtime/GameFramework/Pawn/APawn.h>
#include <Engine/Runtime/Core/Input/Input.h>

namespace Engine
{
    UTrajectoryComponent::UTrajectoryComponent() : m_TrajectoryStaticMesh("Assets/Editor/Object/apawn/trajectory.obj")
    {}

    UTrajectoryComponent::~UTrajectoryComponent() {}

    void UTrajectoryComponent::TickLogic(float deltaTime, const glm::mat4& cameraTransform)
    {
        m_TrajectoryPointArray_Back.push_front(
            {m_nowPosition, m_nowPawnFoward, glm::vec3(0.0f, 0.0f, 0.0f), deltaTime});

        if (m_TrajectoryPointArray_Back.size() > 300)
        {
            m_TrajectoryPointArray_Back.pop_back();
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
            glm::quat(glm::rotate(glm::mat4(1.0f), rotateAngleY + glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

        m_nowCameraRight = glm::rotate(m_nowCameraFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        if (initlized == false)
        {
            m_nowPosition     = glm::vec3(0.0f, 0.0f, 0.0f);
            m_nowMeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);

            m_nowDesiredFoward = m_nowCameraFoward;
            m_nowDesiredRight  = m_nowCameraRight;

            m_nowPawnFoward = m_nowCameraFoward;
            m_nowPawnRight  = m_nowCameraRight;

            m_nowMeshFoward = m_nowCameraFoward;
            m_nowMeshRight  = m_nowCameraRight;

            initlized = true;
        }

        float distance = glm::distance(
            m_TrajectoryPointArray_Back.getTrajectoryPoint(m_TrajectorySampleStep).m_Position, m_nowPosition);
        float pawnMoveSpeed = distance / m_TrajectorySampleStep;

        m_TrajectoryPointArray_Forward.clear();
        m_TrajectoryPointArray_Forward.push_front(
            {m_nowPosition, m_nowPawnFoward, glm::vec3(0.0f, 0.0f, 0.0f), deltaTime});
        for (int i = 1; i < m_TrajectorySampleNum * 2; ++i)
        {
            float nowTime = i * m_TrajectorySampleStep;
            float nowSpeed =
                glm::mix(pawnMoveSpeed, m_DesiredMoveSpeed, glm::min((float)i / 2.0f / m_TrajectorySampleNum, 1.0f));

            glm::quat mixForward =
                glm::slerp(m_nowPawnFoward, m_nowDesiredFoward, (float)i * 5.0f / m_TrajectorySampleNum);
            glm::quat mixRight = glm::slerp(m_nowPawnRight, m_nowDesiredRight, (float)i * 5.0f / m_TrajectorySampleNum);
            glm::vec3 movement = glm::vec3(glm::mat4(mixRight) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

            float           lastTime     = (i - 1) * m_TrajectorySampleStep;
            glm::vec3       lastPosition = m_TrajectoryPointArray_Forward.getTrajectoryPoint(lastTime).m_Position;
            TrajectoryPoint trajectoryPoint {lastPosition + movement * nowSpeed * m_TrajectorySampleStep,
                                             mixForward,
                                             glm::vec3(0.0f, 0.0f, 0.0f),
                                             m_TrajectorySampleStep};

            m_TrajectoryPointArray_Forward.push_back(trajectoryPoint);
        }

        // knn search
        std::deque<TrajectoryPoint> m_TrajecotryPoints_Back;
        std::deque<TrajectoryPoint> m_TrajecotryPoints_Forward;

        // back
        for (int i = 0; i < m_TrajectorySampleNum; ++i)
        {
            float           nowTime         = i * m_TrajectorySampleStep;
            TrajectoryPoint trajectoryPoint = m_TrajectoryPointArray_Back.getTrajectoryPoint(nowTime);

            m_TrajecotryPoints_Back.push_back(trajectoryPoint);
        }

        // forward
        for (int i = 0; i < m_TrajectorySampleNum; ++i)
        {
            float           nowTime         = i * m_TrajectorySampleStep;
            TrajectoryPoint trajectoryPoint = m_TrajectoryPointArray_Forward.getTrajectoryPoint(nowTime);

            m_TrajecotryPoints_Forward.push_back(trajectoryPoint);
        }

        UMotionMatchingComponent& motionMatchingComponent =
            static_cast<APawn*>(m_Owner)->GetMotionMatchingComponentRef();
        USkinnedMeshComponent& skinnedMeshComponent = static_cast<APawn*>(m_Owner)->GetSkinnedMeshComponentRef();

        static float                      accumTime       = 0.0f;
        std::vector<std::array<float, 7>> nowPose         = skinnedMeshComponent.GetNowPose();
        std::vector<JointFeature>         nowJointFeature = skinnedMeshComponent.GetNowJointFeature(0.016f);

        float nowPhase =
            glm::sin(m_SearchResult.nowRatio * motionMatchingComponent.GetFrameTime() * glm::pi<float>() * 2.0f);

        KnnResult result = motionMatchingComponent.Search(
            m_TrajecotryPoints_Back, m_TrajecotryPoints_Forward, nowPose, nowPhase, nowJointFeature);

        if (accumTime > 3.0f || (accumTime > 0.08f && accumTime < 3.0f))
        {
            float nowTime    = m_SearchResult.nowRatio * motionMatchingComponent.GetFrameTime();
            float resultTime = result.nowRatio * motionMatchingComponent.GetFrameTime();
            if (result.index != m_SearchResult.index ||
                result.index == m_SearchResult.index &&
                    ((resultTime - nowTime) < -0.8 || (resultTime - nowTime) > 0.08))
            {
                Log::Info(fmt::format("nowTime: {}, resultTime: {}, loss: {}", nowTime, resultTime, result.loss));
                m_SearchResult = result;
                accumTime      = 0.0f;
            }
        }
        float nowRatio = deltaTime / motionMatchingComponent.GetFrameTime();
        m_SearchResult = {m_SearchResult.index, m_SearchResult.nowRatio + nowRatio};
        motionMatchingComponent.SetNowAnimationClipKnnResult(m_SearchResult);

        Log::Info(fmt::format("AnimationClip: {}, nowRatio: {}", m_SearchResult.index, m_SearchResult.nowRatio));

        accumTime += deltaTime;
    }

    void UTrajectoryComponent::Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform)
    {
        auto&                       vertexArray = m_TrajectoryStaticMesh.GetStaticMesh().m_Meshes[0]->m_VertexArray;
        std::deque<TrajectoryPoint> m_TrajecotryPoints_Back;
        std::deque<TrajectoryPoint> m_TrajecotryPoints_Forward;

        // back
        for (int i = 0; i < m_TrajectorySampleNum; ++i)
        {
            float           nowTime         = i * m_TrajectorySampleStep;
            TrajectoryPoint trajectoryPoint = m_TrajectoryPointArray_Back.getTrajectoryPoint(nowTime);

            m_TrajecotryPoints_Back.push_back(trajectoryPoint);
        }

        // forward
        for (int i = 0; i < m_TrajectorySampleNum; ++i)
        {
            float           nowTime         = i * m_TrajectorySampleStep;
            TrajectoryPoint trajectoryPoint = m_TrajectoryPointArray_Forward.getTrajectoryPoint(nowTime);

            m_TrajecotryPoints_Forward.push_back(trajectoryPoint);
        }

        shader->Bind();

        // draw axis back
        for (int i = 1; i < m_TrajectorySampleNum; ++i)
        {
            float           nowTime         = i * m_TrajectorySampleStep;
            TrajectoryPoint trajectoryPoint = m_TrajecotryPoints_Back[i];

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

        // draw axis forward
        for (int i = 1; i < m_TrajectorySampleNum; ++i)
        {
            float           nowTime         = i * m_TrajectorySampleStep;
            TrajectoryPoint trajectoryPoint = m_TrajecotryPoints_Forward[i];

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

        // draw nowPawnFoward (x axis)
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowPosition);
            glm::mat4 rotation    = glm::toMat4(glm::quat(m_nowPawnFoward));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            // show z axis
            rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * rotation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.0f, 0.0f));

            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
            vertexArray->UnBind();
        }

        // draw nowCameraFoward (x axis)
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowPosition);
            glm::mat4 rotation    = glm::toMat4(glm::quat(m_nowCameraFoward));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            // show z axis
            rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * rotation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.0f, 0.0f, 0.9f));

            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
            vertexArray->UnBind();
        }

        // draw nowDesiredFoward (x axis)
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowPosition);
            glm::mat4 rotation    = glm::toMat4(glm::quat(m_nowDesiredFoward));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            // show z axis
            rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * rotation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.9f, 0.9f));

            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
            vertexArray->UnBind();
        }

        // draw trajectory back
        for (int i = 0; i < m_TrajectorySampleNum - 1; ++i)
        {
            float nowTime  = i * m_TrajectorySampleStep;
            float nextTime = (i + 1) * m_TrajectorySampleStep;

            TrajectoryPoint trajectoryPoint     = m_TrajecotryPoints_Back[i];
            TrajectoryPoint nextTrajectoryPoint = m_TrajecotryPoints_Back[i + 1];

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

        // draw trajectory forward
        for (int i = 0; i < m_TrajectorySampleNum - 1; ++i)
        {
            float nowTime  = i * m_TrajectorySampleStep;
            float nextTime = (i + 1) * m_TrajectorySampleStep;

            TrajectoryPoint trajectoryPoint     = m_TrajecotryPoints_Forward[i];
            TrajectoryPoint nextTrajectoryPoint = m_TrajecotryPoints_Forward[i + 1];

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
                                                  ->m_VertexArray;
            pawnStaticMeshvertexArray->Bind();
            RenderCommand::DrawIndexed(pawnStaticMeshvertexArray);

            pawnStaticMeshvertexArray->UnBind();
        }

        // draw nowMeshFoward (x axis)
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowMeshPosition);
            glm::mat4 rotation    = glm::toMat4(glm::quat(m_nowMeshFoward));
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            // show z axis
            rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * rotation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.3f, 0.6f));

            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
            vertexArray->UnBind();
        }

        // draw nowMeshPosition
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowMeshPosition);
            glm::mat4 scale       = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", translation * scale);
            shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.3f, 0.9f));

            auto& pawnStaticMeshvertexArray = static_cast<APawn*>(m_Owner)
                                                  ->GetPawnComponentRef()
                                                  .GetPawnStaticMeshRef()
                                                  .GetStaticMesh()
                                                  .m_Meshes[0]
                                                  ->m_VertexArray;
            pawnStaticMeshvertexArray->Bind();
            RenderCommand::DrawIndexed(pawnStaticMeshvertexArray);

            pawnStaticMeshvertexArray->UnBind();
        }

        shader->UnBind();
    }

    UPawnComponent::UPawnComponent() : m_PawnStaticMesh("Assets/Editor/Object/apawn/apawn.obj") {}

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
        glm::vec3 forward  = glm::vec3(glm::mat4(nowPawnRight) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

        bool isMoved = false;
        if (actor->GetIsControlled())
        {
            if (Input::IsKeyPressed(GLFW_KEY_W))
            {
                const glm::quat direction = glm::rotate(cameraFoward, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                desiredFoward = glm::slerp(desiredFoward, direction, glm::min(7.0f * deltaTime, 1.0f));
                isMoved       = true;
            }
            if (Input::IsKeyPressed(GLFW_KEY_S))
            {
                const glm::quat direction =
                    glm::rotate(cameraFoward, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                desiredFoward = glm::slerp(desiredFoward, direction, glm::min(7.0f * deltaTime, 1.0f));
                isMoved       = true;
            }
            if (Input::IsKeyPressed(GLFW_KEY_A))
            {
                const glm::quat direction =
                    glm::rotate(cameraFoward, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                desiredFoward = glm::slerp(desiredFoward, direction, glm::min(7.0f * deltaTime, 1.0f));
                isMoved       = true;
            }
            if (Input::IsKeyPressed(GLFW_KEY_D))
            {
                const glm::quat direction =
                    glm::rotate(cameraFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

                desiredFoward = glm::slerp(desiredFoward, direction, glm::min(7.0f * deltaTime, 1.0f));
                isMoved       = true;
            }

            if (isMoved == true)
            {
                movement += forward;
                movement = glm::normalize(movement);
                trajectoryComponent.SetDesiredMoveSpeed(m_PawnMoveSpeed);
            }
            else
            {
                trajectoryComponent.SetDesiredMoveSpeed(0.0f);
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
            desiredFoward = glm::slerp(desiredFoward, pawnFoward, glm::min(7.0f * deltaTime, 1.0f));
        }
        desiredRight = glm::rotate(desiredFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        pawnPosition = glm::mix(pawnPosition, worldPosition, glm::min(10.0f * deltaTime, 1.0f));

        pawnFoward = glm::slerp(pawnFoward, desiredFoward, glm::min(10.0f * deltaTime, 1.0f));
        pawnRight  = glm::rotate(pawnFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        // update SkinnedMesh
        glm::vec3& meshPosition = trajectoryComponent.GetNowMeshPositionRef();
        glm::quat& meshFoward   = trajectoryComponent.GetNowMeshFowardRef();
        glm::quat& meshRight    = trajectoryComponent.GetNowMeshRightRef();

        meshPosition = glm::mix(meshPosition, worldPosition, glm::min(7.0f * deltaTime, 1.0f));
        meshFoward   = glm::slerp(meshFoward, desiredFoward, glm::min(7.0f * deltaTime, 1.0f));
        meshRight    = glm::rotate(meshFoward, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

        USkinnedMeshComponent& skinnedMeshComponent = aPawn->GetSkinnedMeshComponentRef();

        skinnedMeshComponent.GetTransformComponentRef().SetPosition(meshPosition);
        // skinnedMeshComponent.GetTransformComponentRef().SetRotation(glm::eulerAngles(meshFoward));

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

        // update skinnedMesh
        UMotionMatchingComponent&        motionMatchingComponent = aPawn->GetMotionMatchingComponentRef();
        ozz::vector<ozz::math::Float4x4> nowModels               = motionMatchingComponent.GetNowAnimationClipModels();
        // skinnedMeshComponent.SetModels(nowModels);

        skinnedMeshComponent.Update(trajectoryComponent.GetSearchResult().nowRatio);
        skinnedMeshComponent.SetModels(nowModels);
    }

    void UPawnComponent::Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform)
    {
        shader->Bind();

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));

        shader->SetMat4("u_ViewProjection", vpMat);
        shader->SetMat4("u_Transform", transform * scale);
        shader->SetFloat3("u_Color", glm::vec3(0.9f, 0.3f, 0.3f));

        auto& vertexArray = m_PawnStaticMesh.GetStaticMesh().m_Meshes[0]->m_VertexArray;
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);

        vertexArray->UnBind();

        shader->UnBind();
    }

} // namespace Engine