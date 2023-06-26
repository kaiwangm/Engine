#include "USkeletonComponent.h"

namespace Engine
{
    bool loadSkeleton(std::string filename, ozz::animation::Skeleton* skeleton)
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

    bool loadAnimation(std::string filename, ozz::animation::Animation* animation)
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

    USkeletonComponent::USkeletonComponent()
    {
        loadSkeleton("Assert/Animation/skeleton.ozz", &skeleton);
        loadAnimation("Assert/Animation/animation.ozz", &animation);

        num_soa_joints = skeleton.num_soa_joints();
        num_joints     = skeleton.num_joints();

        locals.resize(num_soa_joints);
        models.resize(num_joints);
    }

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

    void USkeletonComponent::Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform)
    {
        auto models_sp = make_span(models);
        for (int i = 0; i < num_joints; i++)
        {
            const ozz::math::Float4x4& model_mat = models_sp[i];
            glm::mat4                  model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));

            model = transform * model;

            shader->Bind();

            shader->SetMat4("u_ViewProjection", vpMat);
            shader->SetMat4("u_Transform", model);
            shader->SetFloat3("color", glm::vec3(1.0f, 0.0f, 0.0f));

            auto& vertexArray = m_Mesh.GetStaticMesh().m_Meshes[0].m_VertexArray;
            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);

            vertexArray->UnBind();

            shader->UnBind();
        }
    }
} // namespace Engine