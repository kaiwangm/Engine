#include <Engine/Runtime/GameFramework/Animation/USkeletonComponent.h>

#include <ozz/base/io/archive.h>
#include <ozz/base/io/stream.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/base/maths/vec_float.h>
#include <ozz/base/maths/soa_transform.h>
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

    USkeletonComponent::USkeletonComponent() :
        m_Joints("Assert/Object/bone/joint.obj"), m_Bones("Assert/Object/bone/bone.obj")
    {
        loadSkeleton("Assert/Animation/skeleton.ozz", &skeleton);
        loadAnimation("Assert/Animation/animation.ozz", &animation);

        num_soa_joints = skeleton.num_soa_joints();
        num_joints     = skeleton.num_joints();

        locals.resize(num_soa_joints);
        models.resize(num_joints);
        parents.resize(num_joints);

        auto joint_parents = skeleton.joint_parents();

        for (int i = 0; i < num_joints; i++)
        {
            parents[i] = joint_parents[i];
        }
    }

    USkeletonComponent::~USkeletonComponent() {}

    void USkeletonComponent::Update(float ratio)
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

        // Converts from local space to model space matrices.
        ozz::animation::LocalToModelJob ltm_job;
        ltm_job.skeleton = &skeleton;
        ltm_job.input    = ozz::make_span(locals);
        ltm_job.output   = ozz::make_span(models);
        if (!ltm_job.Run())
        {
            Log::Error("LocalToModelJob Run Failed.");
        }
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

            auto& vertexArray = m_Joints.GetStaticMesh().m_Meshes[0].m_VertexArray;
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

            auto& vertexArray = m_Bones.GetStaticMesh().m_Meshes[0].m_VertexArray;
            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);

            vertexArray->UnBind();

            shader->UnBind();
        }
    }

    int USkeletonComponent::GetNumJoints() { return num_joints; }
} // namespace Engine