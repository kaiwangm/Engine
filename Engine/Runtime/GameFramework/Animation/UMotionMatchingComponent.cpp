#include <Engine/Runtime/GameFramework/Animation/UMotionMatchingComponent.h>
#include <Engine/Runtime/GameFramework/Pawn/APawn.h>

namespace Engine
{
    void UMotionMatchingComponent::DrawTrajectory(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform)
    {
        auto&                       vertexArray = m_TrajectoryStaticMesh.GetStaticMesh().m_Meshes[0].m_VertexArray;
        std::deque<TrajectoryPoint> m_TrajecotryPoints_Back;
        std::deque<TrajectoryPoint> m_TrajecotryPoints_Forward;

        for (int i = 0; i < m_TrajectorySampleNum; ++i)
        {
            float           sampleTime      = fmod(m_nowTime, m_FrameTime) - i * m_TrajectorySampleStep;
            TrajectoryPoint trajectoryPoint = m_TrajectoryPointArray.getTrajectoryPoint(sampleTime);
            m_TrajecotryPoints_Back.push_back(trajectoryPoint);
        }

        for (int i = 0; i < m_TrajectorySampleNum; ++i)
        {
            float           sampleTime      = fmod(m_nowTime, m_FrameTime) + i * m_TrajectorySampleStep;
            TrajectoryPoint trajectoryPoint = m_TrajectoryPointArray.getTrajectoryPoint(sampleTime);
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

        TrajectoryPoint trajectoryPoint = m_TrajecotryPoints_Back[0];
        glm::vec3       m_nowPosition   = trajectoryPoint.m_Position;
        glm::quat       m_nowFoward     = trajectoryPoint.m_Orientation;

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

        // draw nowMeshFoward (x axis)
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_nowPosition);
            glm::mat4 rotation    = glm::toMat4(m_nowFoward);
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

        shader->UnBind();
    }
} // namespace Engine