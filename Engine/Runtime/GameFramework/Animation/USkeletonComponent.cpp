#include <Engine/Runtime/GameFramework/Animation/USkeletonComponent.h>
#include <ozz/base/io/archive.h>
#include <ozz/base/io/stream.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/base/maths/vec_float.h>
#include <ozz/base/maths/quaternion.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/span.h>

namespace Engine
{
    bool USkeletonComponent::loadSkeleton(const std::string filename, ozz::animation::Skeleton* skeleton)
    {
        ozz::io::File file(filename.c_str(), "rb");
        if (!file.opened())
        {
            return false;
        }
        ozz::io::IArchive archive(&file);

        if (!archive.TestTag<ozz::animation::Skeleton>())
        {
            return false;
        }

        archive >> *skeleton;

        return true;
    }

    bool USkeletonComponent::loadAnimation(const std::string filename, ozz::animation::Animation* animation)
    {
        ozz::io::File file(filename.c_str(), "rb");
        if (!file.opened())
        {
            return false;
        }
        ozz::io::IArchive archive(&file);

        if (!archive.TestTag<ozz::animation::Animation>())
        {
            return false;
        }

        archive >> *animation;

        return true;
    }

    void USkeletonComponent::InitSkeleton(const std::string& skeletonPath, const std::string& animationPath)
    {
        loadSkeleton(skeletonPath, &skeleton);
        loadAnimation(animationPath, &animation);

        num_soa_joints = skeleton.num_soa_joints();
        num_joints     = skeleton.num_joints();

        locals.resize(num_soa_joints);
        models.resize(num_joints);
        parents.resize(num_joints);

        auto joint_parents = skeleton.joint_parents();
        auto joint_names   = skeleton.joint_names();

        for (int i = 0; i < num_joints; i++)
        {
            parents[i] = joint_parents[i];
            m_JointNames.push_back(joint_names[i]);

            std::vector<std::string> keyWords {
                "Spine",
                "Neck",
                "Head",
                "Shoulder",
                "Arm",
                "ForeArm",
                "UpLeg",
                "Leg",
                "Foot",
                "Toe",
            };

            bool isFind = false;
            for (int j = 0; j < keyWords.size(); ++j)
            {
                if (m_JointNames[i].find(keyWords[j]) != std::string::npos)
                {
                    isFind = true;
                    break;
                }
            }

            if (isFind)
            {
                m_JointTypeIndex.push_back(1);
            }
            else
            {
                m_JointTypeIndex.push_back(0);
            }
        }

        m_FrameTime = animation.duration();
    }

    USkeletonComponent::USkeletonComponent() :
        m_Joints("Assets/Editor/Object/bone/joint.obj"), m_Bones("Assets/Editor/Object/bone/bone.obj")
    {
        InitSkeleton("Assets/Editor/Animation/skeleton.ozz", "Assets/Editor/Animation/animation.ozz");
    }

    USkeletonComponent::USkeletonComponent(const std::string& skeletonPath, const std::string& animationPath) :
        m_Joints("Assets/Editor/Object/bone/joint.obj"), m_Bones("Assets/Editor/Object/bone/bone.obj")
    {
        InitSkeleton(skeletonPath, animationPath);
    }

    USkeletonComponent::~USkeletonComponent() {}

    void USkeletonComponent::Update(float ratio, bool useRootMotion)
    {
        ozz::animation::SamplingJob::Context context;
        context.Resize(num_joints);

        ozz::animation::SamplingJob sampling_job;
        sampling_job.animation = &animation;
        sampling_job.context   = &context;
        sampling_job.ratio     = ratio;
        sampling_job.output    = ozz::make_span(locals);
        if (!sampling_job.Run())
        {
            Log::Error("SamplingJob Run Failed.");
        }

        static bool                     initlized = false;
        static ozz::math::SoaFloat3     initlized_translations;
        static ozz::math::SoaQuaternion initlized_rotations;
        if (!initlized)
        {
            ozz::span<const ozz::math::SoaTransform> restpsed = skeleton.joint_rest_poses();

            initlized_translations = restpsed[0].translation;
            initlized_rotations    = restpsed[0].rotation;
            initlized              = true;
        }
        else
        {
            if (m_UseRootMotion == false && useRootMotion == false)
            {
                locals[0].translation = initlized_translations;
                locals[0].rotation    = initlized_rotations;
            }
            else if (useRootMotion == true)
            {
                // locals[0].translation = initlized_translations;
                // locals[0].rotation    = initlized_rotations;
            }
        }

        // Converts from local space to model space matrices.
        ozz::animation::LocalToModelJob ltm_job;
        ltm_job.skeleton = &skeleton;
        ltm_job.input    = ozz::make_span(locals);
        ltm_job.output   = ozz::make_span(models);
        if (!ltm_job.Run())
        {
            Log::Error("LocalToModelJob Run Failed.");
        }

        m_NowTime = ratio * m_FrameTime;
    }

    bool USkeletonComponent::GetLocals(ozz::vector<ozz::math::SoaTransform>& inLocals, float ratio)
    {
        ozz::animation::SamplingJob::Context context;
        context.Resize(num_joints);

        inLocals.resize(num_soa_joints);

        ozz::animation::SamplingJob sampling_job;
        sampling_job.animation = &animation;
        sampling_job.context   = &context;
        sampling_job.ratio     = ratio;
        sampling_job.output    = ozz::make_span(inLocals);

        if (!sampling_job.Run())
        {
            Log::Error("SamplingJob Run Failed.");
            return false;
        }

        static bool                     initlized = false;
        static ozz::math::SoaFloat3     initlized_translations;
        static ozz::math::SoaQuaternion initlized_rotations;
        if (!initlized)
        {
            ozz::span<const ozz::math::SoaTransform> restpsed = skeleton.joint_rest_poses();

            initlized_translations = restpsed[0].translation;
            initlized_rotations    = restpsed[0].rotation;
            initlized              = true;
        }

        inLocals[0].translation = initlized_translations;

        return true;
    }

    void USkeletonComponent::Draw(Ref<Shader> shader,
                                  glm::mat4   vpMat,
                                  glm::mat4   transform,
                                  glm::vec3   viewPos,
                                  glm::vec3   color,
                                  glm::vec3   lightColor)
    {
        auto               models_sp = make_span(models);
        std::vector<float> bones_length;
        bones_length.resize(num_joints, 1.0f);
        std::vector<float> joints_length;
        joints_length.resize(num_joints, 0.03f);

        for (int i = 0; i < num_joints; i++)
        {
            if (parents[i] == -1)
            {
                continue;
            }

            const ozz::math::Float4x4& model_mat = models_sp[i];
            glm::mat4                  model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));

            const ozz::math::Float4x4& parent_model_mat = models_sp[parents[i]];
            glm::mat4                  parent_model;
            memcpy(&parent_model, &parent_model_mat.cols[0], sizeof(glm::mat4));

            bones_length[i] = glm::length(glm::vec3(model[3][0], model[3][1], model[3][2]) -
                                          glm::vec3(parent_model[3][0], parent_model[3][1], parent_model[3][2]));
        }

        for (int i = 0; i < num_joints; i++)
        {
            if (parents[i] == -1)
            {
                continue;
            }

            const ozz::math::Float4x4& model_mat = models_sp[i];
            glm::mat4                  model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));

            const ozz::math::Float4x4& parent_model_mat = models_sp[parents[i]];
            glm::mat4                  parent_model;
            memcpy(&parent_model, &parent_model_mat.cols[0], sizeof(glm::mat4));

            joints_length[parents[i]] =
                glm::length(glm::vec3(model[3][0], model[3][1], model[3][2]) -
                            glm::vec3(parent_model[3][0], parent_model[3][1], parent_model[3][2]));
        }

        for (int i = 0; i < num_joints; i++)
        {
            const ozz::math::Float4x4& model_mat = models_sp[i];
            glm::mat4                  model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));

            model = transform * model;

            shader->Bind();

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", model);
            shader->SetFloat3("ViewPos", viewPos);
            shader->SetFloat3("u_Color", color);
            shader->SetFloat("u_BoneLength", 1.8 * joints_length[i]);

            auto& vertexArray = m_Joints.GetStaticMesh().m_Meshes[0]->m_VertexArray;
            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);

            vertexArray->UnBind();

            shader->UnBind();
        }

        for (int i = 0; i < num_joints; i++)
        {
            if (parents[i] == -1)
            {
                continue;
            }

            const ozz::math::Float4x4& model_mat = models_sp[parents[i]];
            glm::mat4                  model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));

            model = transform * model;

            shader->Bind();

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", model);
            shader->SetFloat3("ViewPos", viewPos);
            shader->SetFloat3("u_Color", color);
            shader->SetFloat("u_BoneLength", bones_length[i]);

            auto& vertexArray = m_Bones.GetStaticMesh().m_Meshes[0]->m_VertexArray;
            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);

            vertexArray->UnBind();

            shader->UnBind();
        }
    }

    int USkeletonComponent::GetNumJoints() { return num_joints; }

    std::vector<std::array<float, 7>> USkeletonComponent::GetNowPose()
    {
        std::vector<std::array<float, 7>> result;

        for (int i = 0; i < locals.size(); ++i)
        {
            std::function<float(const ozz::math::SimdFloat4&)> getFloat = [&](const ozz::math::SimdFloat4& simdFloat4) {
                float result[4];
                _mm_store_ps(result, simdFloat4);
                return result[0];
            };

            std::array<float, 7> joint {
                getFloat(locals[i].translation.x),
                getFloat(locals[i].translation.y),
                getFloat(locals[i].translation.z),
                getFloat(locals[i].rotation.x),
                getFloat(locals[i].rotation.y),
                getFloat(locals[i].rotation.z),
                getFloat(locals[i].rotation.w),
            };

            result.push_back(joint);
        }

        return result;
    }

    std::vector<JointFeature> USkeletonComponent::GetNowJointFeature(float deltaTime)
    {
        float nowRatio  = m_NowTime / m_FrameTime;
        float lastRatio = glm::max(m_NowTime - deltaTime, 0.0f) / m_FrameTime;

        std::vector<JointFeature> result(num_joints);
        std::vector<glm::vec3>    nowRootSpacePosition(num_joints);
        std::vector<glm::vec3>    lastRootSpacePosition(num_joints);

        Update(nowRatio, false);
        glm::vec3 nowRootPosition;
        {
            const ozz::math::Float4x4& model_mat = models[0];
            glm::mat4                  model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));
            nowRootPosition = glm::vec3(model[3][0], model[3][1], model[3][2]);
            for (int i = 0; i < num_joints; ++i)
            {
                const ozz::math::Float4x4& model_mat = models[i];
                glm::mat4                  model;
                memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));
                nowRootSpacePosition[i] = glm::vec3(model[3][0], model[3][1], model[3][2]) - nowRootPosition;
            }
            // nowRootSpacePosition[0] = glm::vec3(0.0f);
        }

        Update(lastRatio, false);
        glm::vec3 lastRootPosition;
        {
            const ozz::math::Float4x4& model_mat = models[0];
            glm::mat4                  model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));
            lastRootPosition = glm::vec3(model[3][0], model[3][1], model[3][2]);
            for (int i = 0; i < num_joints; ++i)
            {
                const ozz::math::Float4x4& model_mat = models[i];
                glm::mat4                  model;
                memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));
                lastRootSpacePosition[i] = glm::vec3(model[3][0], model[3][1], model[3][2]) - lastRootPosition;
            }
            // lastRootSpacePosition[0] = glm::vec3(0.0f);
        }

        for (int i = 0; i < num_joints; ++i)
        {
            result[i].rootSpacePosition = nowRootSpacePosition[i];
            result[i].rootSpaceVelocity = (nowRootSpacePosition[i] - lastRootSpacePosition[i]) / deltaTime;
        }

        result[0].rootSpaceVelocity = (nowRootPosition - lastRootPosition) / deltaTime;

        Update(nowRatio);

        return result;
    }
} // namespace Engine