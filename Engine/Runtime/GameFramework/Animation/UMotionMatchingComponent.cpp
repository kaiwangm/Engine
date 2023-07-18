#include <Engine/Runtime/GameFramework/Animation/UMotionMatchingComponent.h>
#include <Engine/Runtime/GameFramework/Pawn/APawn.h>
#include <hnswlib/hnswlib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace Engine
{
    void UMotionMatchingComponent::SaveAsJson(const std::vector<std::vector<JointFeature>>& nowJointFeatures,
                                              const std::string&                            path)
    {
        rapidjson::Document document;
        document.SetObject();

        // add array to document
        document.AddMember("num_frame", nowJointFeatures.size(), document.GetAllocator());
        document.AddMember("num_channels", dimPoseFeature, document.GetAllocator());

        rapidjson::Value array(rapidjson::kArrayType);
        for (int i = 0; i < nowJointFeatures.size(); ++i)
        {
            rapidjson::Value frame(rapidjson::kObjectType);

            rapidjson::Value features(rapidjson::kArrayType);
            features.PushBack(0.0f, document.GetAllocator());
            features.PushBack(0.0f, document.GetAllocator());
            features.PushBack(0.0f, document.GetAllocator());
            for (int j = 1; j < nowJointFeatures[i].size(); ++j)
            {
                features.PushBack(nowJointFeatures[i][j].rootSpaceVelocity.x, document.GetAllocator());
                features.PushBack(nowJointFeatures[i][j].rootSpaceVelocity.y, document.GetAllocator());
                features.PushBack(nowJointFeatures[i][j].rootSpaceVelocity.z, document.GetAllocator());
            }
            frame.AddMember("features", features, document.GetAllocator());

            rapidjson::Value metadata(rapidjson::kObjectType);
            metadata.AddMember("sequence_index", 1, document.GetAllocator());
            metadata.AddMember("nowframe_index", i, document.GetAllocator());
            metadata.AddMember("frame_type", "Standard", document.GetAllocator());
            metadata.AddMember("file_name", "Recording_001.bvh", document.GetAllocator());
            metadata.AddMember("hash", "aaaaaa", document.GetAllocator());
            frame.AddMember("metadata", metadata, document.GetAllocator());

            array.PushBack(frame, document.GetAllocator());
        }
        document.AddMember("rawdata", array, document.GetAllocator());

        // write to file
        rapidjson::StringBuffer                    buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        std::ofstream ofs(path);
        ofs << buffer.GetString();
        ofs.close();
    }

    void UMotionMatchingComponent::LoadPhaseManifold(const std::string& path)
    {
        std::vector<std::array<float, dimDeepPhase>> data_raw_deep_phase {};
        std::ifstream                                ifs(path);
        std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

        rapidjson::Document document;
        document.Parse(str.c_str());

        const rapidjson::Value& num_frame    = document["num_frame"];
        const rapidjson::Value& num_channels = document["num_channels"];
        const rapidjson::Value& rawdata      = document["rawdata"];

        const int numFrame    = num_frame.GetInt();
        const int numChannels = num_channels.GetInt();

        data_raw_deep_phase.resize(numFrame);
        data_raw_deep_phase.assign(numFrame, std::array<float, dimDeepPhase> {});

        for (int i = 0; i < numFrame; ++i)
        {
            const rapidjson::Value& frame = rawdata[i];

            const rapidjson::Value& features = frame["manifold"];
            for (int j = 0; j < numChannels; ++j)
            {
                data_raw_deep_phase[i][j] = features[j].GetFloat();
            }
        }

        ifs.close();

        for (int i = 0; i < numFrame; ++i)
        {
            std::array<float, dimDeepPhase> sample  = data_raw_deep_phase[i];
            float                           nowtime = (i + 1) * m_SampleStep;

            ManiflowPoint<dimDeepPhase> maniflowPoint {
                sample,
                nowtime,
            };

            m_ManiflowArray.push_back(maniflowPoint);
        }
    }

    void UMotionMatchingComponent::Initialize()
    {
        m_FrameTime  = m_SkinnedMesh.GetFrameTime();
        m_SampleStep = 1.0f / 24.0f;
        for (float nowtime = 0.0f; nowtime < m_FrameTime; nowtime += m_SampleStep)
        {
            float ratio = (fmod(nowtime, m_FrameTime)) / m_FrameTime;

            m_SkinnedMesh.Update(ratio);

            TrajectoryPoint point {
                m_SkinnedMesh.GetNowRootPosition(),
                m_SkinnedMesh.GetNowRootOrientation(),
                glm::vec3(0.0f),
                m_SampleStep,
            };

            m_TrajectoryPointArray.push_back(point);
        }

        const float startTime = 0.0f;

        // knn search initialize
        for (float nowtime = startTime; nowtime < m_FrameTime; nowtime += m_SampleStep)
        {
            KnnResult result {0, nowtime / m_FrameTime};
            m_SearchResult.push_back(result);
        }

        std::vector<std::array<float, dimRawTrajectory>> data_raw_trajecotry {};
        std::vector<std::array<float, dimDeepPhase>>     data_raw_pose {};
        std::vector<std::array<float, dimRawPawn>>       data_raw_pawn {};
        std::vector<std::array<float, 1>>                data_raw_phase {};
        std::vector<std::vector<JointFeature>>           nowJointFeatures {};

        LoadPhaseManifold("Assets/Editor/Animation/lafan/deepPhase.json");

        for (float nowtime = startTime; nowtime < m_FrameTime; nowtime += m_SampleStep)
        {
            std::deque<TrajectoryPoint> m_TrajecotryPoints_Back;
            std::deque<TrajectoryPoint> m_TrajecotryPoints_Forward;

            for (int i = 0; i < m_TrajectorySampleNum; ++i)
            {
                float           sampleTime      = fmod(nowtime, m_FrameTime) - i * m_TrajectorySampleStep;
                TrajectoryPoint trajectoryPoint = m_TrajectoryPointArray.getTrajectoryPoint(sampleTime);
                m_TrajecotryPoints_Back.push_back(trajectoryPoint);
            }

            for (int i = 0; i < m_TrajectorySampleNum; ++i)
            {
                float           sampleTime      = fmod(nowtime, m_FrameTime) + i * m_TrajectorySampleStep;
                TrajectoryPoint trajectoryPoint = m_TrajectoryPointArray.getTrajectoryPoint(sampleTime);
                m_TrajecotryPoints_Forward.push_back(trajectoryPoint);
            }

            glm::vec3 rootPosition = m_TrajecotryPoints_Forward[0].m_Position;

            std::array<float, dimRawTrajectory> sampleTrajectory {};
            for (int i = 0; i < m_TrajectorySampleNum; ++i)
            {
                TrajectoryPoint& trajectoryPoint = m_TrajecotryPoints_Back[i];

                sampleTrajectory[i * 10 + 0] = 0.0f;
                sampleTrajectory[i * 10 + 1] = 0.0f;
                sampleTrajectory[i * 10 + 2] = 0.0f;
                sampleTrajectory[i * 10 + 3] = trajectoryPoint.m_Orientation.x;
                sampleTrajectory[i * 10 + 4] = trajectoryPoint.m_Orientation.y;
                sampleTrajectory[i * 10 + 5] = trajectoryPoint.m_Orientation.z;
                sampleTrajectory[i * 10 + 6] = trajectoryPoint.m_Orientation.w;
                sampleTrajectory[i * 10 + 7] = trajectoryPoint.m_Velocity.x;
                sampleTrajectory[i * 10 + 8] = trajectoryPoint.m_Velocity.y;
                sampleTrajectory[i * 10 + 9] = trajectoryPoint.m_Velocity.z;
            }

            for (int i = m_TrajectorySampleNum; i < m_TrajectorySampleNum * 2; ++i)
            {
                TrajectoryPoint& trajectoryPoint = m_TrajecotryPoints_Forward[i - m_TrajectorySampleNum];

                sampleTrajectory[i * 10 + 0] = 0.0f;
                sampleTrajectory[i * 10 + 1] = 0.0f;
                sampleTrajectory[i * 10 + 2] = 0.0f;
                sampleTrajectory[i * 10 + 3] = trajectoryPoint.m_Orientation.x;
                sampleTrajectory[i * 10 + 4] = trajectoryPoint.m_Orientation.y;
                sampleTrajectory[i * 10 + 5] = trajectoryPoint.m_Orientation.z;
                sampleTrajectory[i * 10 + 6] = trajectoryPoint.m_Orientation.w;
                sampleTrajectory[i * 10 + 7] = trajectoryPoint.m_Velocity.x;
                sampleTrajectory[i * 10 + 8] = trajectoryPoint.m_Velocity.y;
                sampleTrajectory[i * 10 + 9] = trajectoryPoint.m_Velocity.z;
            }
            data_raw_trajecotry.push_back(sampleTrajectory);

            // std::array<float, dimRawPose> samplePose {};
            std::array<float, dimDeepPhase> samplePose {};

            m_SkinnedMesh.Update(nowtime / m_FrameTime);

            std::vector<std::array<float, 7>> nowPose = m_SkinnedMesh.GetNowPose();
            // std::vector<JointFeature>         nowJointFeature = m_SkinnedMesh.GetNowJointFeature(0.016f);
            // nowJointFeatures.push_back(nowJointFeature);

            // for (int i = 1; i < nowJointFeature.size(); ++i)
            // {
            //     samplePose[i * 7 + 0] = 0.0f;
            //     samplePose[i * 7 + 1] = 0.0f;
            //     samplePose[i * 7 + 2] = 0.0f;
            //     samplePose[i * 7 + 3] = 0.0f;
            //     samplePose[i * 7 + 4] = nowJointFeature[i].rootSpaceVelocity.x;
            //     samplePose[i * 7 + 5] = nowJointFeature[i].rootSpaceVelocity.y;
            //     samplePose[i * 7 + 6] = nowJointFeature[i].rootSpaceVelocity.z;
            // }

            ManiflowPoint<dimDeepPhase> maniflowPoint = m_ManiflowArray.getManiflowPoint(nowtime);
            for (int i = 0; i < dimDeepPhase; ++i)
            {
                samplePose[i] = maniflowPoint.m_Maniflow[i];
            }
            data_raw_pose.push_back(samplePose);

            std::array<float, dimRawPawn> samplePawn {};
            {
                TrajectoryPoint& trajectoryPoint = m_TrajecotryPoints_Forward[0];
                samplePawn[0]                    = 0.0f;
                samplePawn[1]                    = 0.0f;
                samplePawn[2]                    = 0.0f;
                samplePawn[3]                    = nowPose[0][3];
                samplePawn[4]                    = nowPose[0][4];
                samplePawn[5]                    = nowPose[0][5];
                samplePawn[6]                    = nowPose[0][6];
            }
            data_raw_pawn.push_back(samplePawn);
        }

        // SaveAsJson(nowJointFeatures, "./nowJointFeatures.json");

        for (float nowtime = startTime; nowtime < m_FrameTime; nowtime += m_SampleStep)
        {
            data_raw_phase.push_back({
                glm::sin(nowtime * glm::pi<float>() * 2.0f),
            });
        }

        // pca initialize
        cv::Mat data_raw_trajecotry_mat(data_raw_trajecotry.size(), dimRawTrajectory, CV_32FC1);
        for (int i = 0; i < data_raw_trajecotry.size(); ++i)
        {
            for (int j = 0; j < dimRawTrajectory; ++j)
            {
                data_raw_trajecotry_mat.at<float>(i, j) = data_raw_trajecotry[i][j];
            }
        }

        cv::Mat data_raw_pose_mat(data_raw_pose.size(), dimDeepPhase, CV_32FC1);
        for (int i = 0; i < data_raw_pose.size(); ++i)
        {
            for (int j = 0; j < dimDeepPhase; ++j)
            {
                data_raw_pose_mat.at<float>(i, j) = data_raw_pose[i][j];
            }
        }

        cv::Mat data_raw_pawn_mat(data_raw_pawn.size(), dimRawPawn, CV_32FC1);
        for (int i = 0; i < data_raw_pawn.size(); ++i)
        {
            for (int j = 0; j < dimRawPawn; ++j)
            {
                data_raw_pawn_mat.at<float>(i, j) = data_raw_pawn[i][j];
            }
        }

        pcaTrajectory = cv::PCA(data_raw_trajecotry_mat, cv::Mat(), cv::PCA::DATA_AS_ROW, dimTrajectory);
        pcaPose       = cv::PCA(data_raw_pose_mat, cv::Mat(), cv::PCA::DATA_AS_ROW, dimPose);
        pcaPawn       = cv::PCA(data_raw_pawn_mat, cv::Mat(), cv::PCA::DATA_AS_ROW, dimPawn);

        cv::Mat transformed_trajecotry = pcaTrajectory.project(data_raw_trajecotry_mat);
        cv::Mat transformed_pose       = pcaPose.project(data_raw_pose_mat);
        cv::Mat transformed_pawn       = pcaPawn.project(data_raw_pawn_mat);

        // mv to data_raw
        for (int i = 0; i < data_raw_trajecotry.size(); ++i)
        {
            std::array<float, dim> sample;
            for (int j = 0; j < dimTrajectory; ++j)
            {
                sample[j] = transformed_trajecotry.at<float>(i, j) * weightTrajectory;
            }
            for (int j = 0; j < dimPose; ++j)
            {
                sample[dimTrajectory + j] = transformed_pose.at<float>(i, j) * weightPose;
            }
            for (int j = 0; j < dimPawn; ++j)
            {
                sample[dimTrajectory + dimPose + j] = transformed_pawn.at<float>(i, j) * weightPawn;
            }
            sample[dimTrajectory + dimPose + dimPawn] = data_raw_phase[i][0] * weightPhase;
            data_raw.push_back(sample);
        }

        // hnsw initialize
        int           max_elements = data_raw.size(); // Maximum number of elements, should be known beforehand
        constexpr int M            = 16;              // Tightly connected with internal dimensionality of the data
                                                      // strongly affects the memory consumption
        constexpr int ef_construction = 200;          // Controls index search speed/build speed tradeoff

        // Initing index
        static hnswlib::L2Space space(dim);
        alg_hnsw = new hnswlib::HierarchicalNSW<float>(&space, max_elements, M, ef_construction);

        // Add data to index
        for (int i = 0; i < max_elements; i++)
        {
            alg_hnsw->addPoint(data_raw[i].data(), i);
        }

        // Query the elements for themselves and measure recall
        float correct = 0;
        for (int i = 0; i < max_elements; i++)
        {
            std::priority_queue<std::pair<float, hnswlib::labeltype>> result =
                alg_hnsw->searchKnn(data_raw[i].data(), 1);
            hnswlib::labeltype label = result.top().second;
            if (label == i)
                correct++;
        }
        float recall = correct / max_elements;
        std::cout << "Recall: " << recall << "\n";
    }

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

    KnnResult UMotionMatchingComponent::Search(const std::deque<TrajectoryPoint>&       trajecotryPointsBack,
                                               const std::deque<TrajectoryPoint>&       trajecotryPointsForward,
                                               const std::vector<std::array<float, 7>>& nowPose,
                                               const float                              nowPhase,
                                               const std::vector<JointFeature>&         nowJointFeature,
                                               const int                                nowAnimationClip,
                                               const float                              nowTime)
    {
        std::array<float, dim> query;
        glm::vec3              rootPosition = trajecotryPointsForward[0].m_Position;

        std::array<float, dimRawTrajectory> data_raw_trajecotry {};
        std::array<float, dimDeepPhase>       data_raw_pose {};
        std::array<float, dimRawPawn>       data_raw_pawn {};
        for (int i = 0; i < m_TrajectorySampleNum; ++i)
        {
            const TrajectoryPoint& trajectoryPoint = trajecotryPointsBack[i];

            data_raw_trajecotry[i * 10 + 0] = 0.0f;
            data_raw_trajecotry[i * 10 + 1] = 0.0f;
            data_raw_trajecotry[i * 10 + 2] = 0.0f;
            data_raw_trajecotry[i * 10 + 3] = trajectoryPoint.m_Orientation.x;
            data_raw_trajecotry[i * 10 + 4] = trajectoryPoint.m_Orientation.y;
            data_raw_trajecotry[i * 10 + 5] = trajectoryPoint.m_Orientation.z;
            data_raw_trajecotry[i * 10 + 6] = trajectoryPoint.m_Orientation.w;
            data_raw_trajecotry[i * 10 + 7] = trajectoryPoint.m_Velocity.x;
            data_raw_trajecotry[i * 10 + 8] = trajectoryPoint.m_Velocity.y;
            data_raw_trajecotry[i * 10 + 9] = trajectoryPoint.m_Velocity.z;
        }

        for (int i = m_TrajectorySampleNum; i < m_TrajectorySampleNum * 2; ++i)
        {
            const TrajectoryPoint& trajectoryPoint = trajecotryPointsForward[i - m_TrajectorySampleNum];

            data_raw_trajecotry[i * 10 + 0] = 0.0f;
            data_raw_trajecotry[i * 10 + 1] = 0.0f;
            data_raw_trajecotry[i * 10 + 2] = 0.0f;
            data_raw_trajecotry[i * 10 + 3] = trajectoryPoint.m_Orientation.x;
            data_raw_trajecotry[i * 10 + 4] = trajectoryPoint.m_Orientation.y;
            data_raw_trajecotry[i * 10 + 5] = trajectoryPoint.m_Orientation.z;
            data_raw_trajecotry[i * 10 + 6] = trajectoryPoint.m_Orientation.w;
            data_raw_trajecotry[i * 10 + 7] = trajectoryPoint.m_Velocity.x;
            data_raw_trajecotry[i * 10 + 8] = trajectoryPoint.m_Velocity.y;
            data_raw_trajecotry[i * 10 + 9] = trajectoryPoint.m_Velocity.z;
        }

        // for (int i = 1; i < nowJointFeature.size(); ++i)
        // {
        //     data_raw_pose[i * 7 + 0] = 0.0f;
        //     data_raw_pose[i * 7 + 1] = 0.0f;
        //     data_raw_pose[i * 7 + 2] = 0.0f;
        //     data_raw_pose[i * 7 + 3] = 0.0f;
        //     data_raw_pose[i * 7 + 4] = nowJointFeature[i].rootSpaceVelocity.x;
        //     data_raw_pose[i * 7 + 5] = nowJointFeature[i].rootSpaceVelocity.y;
        //     data_raw_pose[i * 7 + 6] = nowJointFeature[i].rootSpaceVelocity.z;
        // }

        ManiflowPoint<dimDeepPhase> maniflowPoint = m_ManiflowArray.getManiflowPoint(nowTime);
        for (int i = 0; i < dimDeepPhase; ++i)
        {
            data_raw_pose[i] = maniflowPoint.m_Maniflow[i];
        }

        {
            const TrajectoryPoint& trajectoryPoint = trajecotryPointsForward[0];
            data_raw_pawn[0]                       = 0.0f;
            data_raw_pawn[1]                       = 0.0f;
            data_raw_pawn[2]                       = 0.0f;
            data_raw_pawn[3]                       = trajectoryPoint.m_Orientation.x;
            data_raw_pawn[4]                       = trajectoryPoint.m_Orientation.y;
            data_raw_pawn[5]                       = trajectoryPoint.m_Orientation.z;
            data_raw_pawn[6]                       = trajectoryPoint.m_Orientation.w;
        }

        cv::Mat data_raw_trajecotry_mat(1, dimRawTrajectory, CV_32FC1);
        for (int i = 0; i < dimRawTrajectory; ++i)
        {
            data_raw_trajecotry_mat.at<float>(0, i) = data_raw_trajecotry[i];
        }

        cv::Mat data_raw_pose_mat(1, dimDeepPhase, CV_32FC1);
        for (int i = 0; i < dimDeepPhase; ++i)
        {
            data_raw_pose_mat.at<float>(0, i) = data_raw_pose[i];
        }

        cv::Mat data_raw_pawn_mat(1, dimRawPawn, CV_32FC1);
        for (int i = 0; i < dimRawPawn; ++i)
        {
            data_raw_pawn_mat.at<float>(0, i) = data_raw_pawn[i];
        }

        cv::Mat transformed_trajecotry = pcaTrajectory.project(data_raw_trajecotry_mat);
        cv::Mat transformed_pose       = pcaPose.project(data_raw_pose_mat);
        cv::Mat transformed_pawn       = pcaPawn.project(data_raw_pawn_mat);

        // mv to query
        for (int i = 0; i < dimTrajectory; ++i)
        {
            query[i] = transformed_trajecotry.at<float>(0, i) * weightTrajectory;
        }
        for (int i = 0; i < dimPose; ++i)
        {
            query[dimTrajectory + i] = transformed_pose.at<float>(0, i) * weightPose;
        }
        for (int i = 0; i < dimPawn; ++i)
        {
            query[dimTrajectory + dimPose + i] = transformed_pawn.at<float>(0, i) * weightPawn;
        }
        query[dimTrajectory + dimPose + dimPawn] = nowPhase * weightPhase;

        std::priority_queue<std::pair<float, hnswlib::labeltype>> result = alg_hnsw->searchKnn(query.data(), 1);
        std::vector<std::pair<float, hnswlib::labeltype>>         result_vec;
        while (!result.empty())
        {
            result_vec.push_back(result.top());
            result.pop();
        }

        std::random_device rd;
        std::mt19937       gen(rd());
        std::shuffle(result_vec.begin(), result_vec.end(), gen);

        hnswlib::labeltype label = result_vec[0].second;

        KnnResult knnResult = m_SearchResult[label];
        knnResult.loss      = result_vec[0].first;

        return knnResult;
    }
} // namespace Engine