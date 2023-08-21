#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Core/Log/Log.h>
#include <Engine/Runtime/Renderer/Renderer.h>

#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/base/containers/vector.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/vec_float.h>

#include <Engine/Runtime/GameFramework/StaticMesh/UStaticMeshComponent.h>
#include <vector>

namespace Engine
{
    struct JointFeature
    {
        glm::vec3 rootSpacePosition;
        glm::vec3 rootSpaceVelocity;
    };

    class USkeletonComponent : public UComponent
    {
    private:
        ozz::animation::Skeleton             skeleton;
        ozz::animation::Animation            animation;
        ozz::vector<ozz::math::SoaTransform> locals;
        ozz::vector<ozz::math::Float4x4>     models;
        int                                  num_soa_joints;
        int                                  num_joints;
        std::vector<int>                     parents;
        ozz::math::Float3                    color;

        std::vector<std::string> m_JointNames;
        std::vector<uint32_t>    m_JointTypeIndex;

        float m_FrameTime;
        float m_NowTime;
        bool  m_UseRootMotion = true;

    private:
        UStaticMeshComponent m_Joints;
        UStaticMeshComponent m_Bones;

    private:
        static bool loadSkeleton(const std::string filename, ozz::animation::Skeleton* skeleton);
        static bool loadAnimation(const std::string filename, ozz::animation::Animation* animation);
        void        InitSkeleton(const std::string& skeletonPath, const std::string& animationPath);

    public:
        USkeletonComponent();
        USkeletonComponent(const std::string& skeletonPath, const std::string& animationPath);
        ~USkeletonComponent();

    public:
        void Update(float ratio, bool useRootMotion = false);
        bool GetLocals(ozz::vector<ozz::math::SoaTransform>& inLocals, float ratio);
        void Draw(Ref<Shader> shader,
                  glm::mat4   vpMat,
                  glm::mat4   transform,
                  glm::vec3   viewPos,
                  glm::vec3   color,
                  glm::vec3   lightColor);

    public:
        int                               GetNumJoints();
        ozz::vector<ozz::math::Float4x4>& GetModelsRef() { return models; }
        float                             GetFrameTime() const { return m_FrameTime; }

    public:
        bool m_ShowSkeleton = true;

    public:
        bool  GetShowSkeleton() const { return m_ShowSkeleton; }
        bool& GetShowSkeletonRef() { return m_ShowSkeleton; }
        void  SetShowSkeleton(bool showSkeleton) { m_ShowSkeleton = showSkeleton; }
        bool  GetUseRootMotion() const { return m_UseRootMotion; }
        bool& GetUseRootMotionRef() { return m_UseRootMotion; }
        void  SetUseRootMotion(bool useRootMotion) { m_UseRootMotion = useRootMotion; }

        void SetModels(const ozz::vector<ozz::math::Float4x4>& models) { this->models = models; }
        ozz::vector<ozz::math::Float4x4> GetModels() const { return models; }

        std::string GetJointName(int index) const { return m_JointNames[index]; }
        uint32_t    GetJointTypeIndex(int index) const { return m_JointTypeIndex[index]; }

        ozz::animation::Skeleton& GetSkeletonRef() { return skeleton; }

    public:
        glm::vec3 GetNowRootPosition() const
        {
            auto                       models_sp = make_span(models);
            const ozz::math::Float4x4& model_mat = models_sp[0];

            glm::mat4 model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));

            float x = model[3][0];
            float y = model[3][1];
            float z = model[3][2];

            return glm::vec3(x, y, z);
        }

        glm::quat GetNowRootOrientation() const
        {
            auto                       models_sp = make_span(models);
            const ozz::math::Float4x4& model_mat = models_sp[0];

            glm::mat4 model;
            memcpy(&model, &model_mat.cols[0], sizeof(glm::mat4));

            glm::quat q = glm::quat_cast(model);

            return q;
        }

        glm::vec3 GetVelocity(float deltaTime)
        {
            float nowRatio  = m_NowTime / m_FrameTime;
            float lastRatio = glm::max(m_NowTime - deltaTime, 0.0f) / m_FrameTime;

            std::vector<JointFeature> result(num_joints);
            std::vector<glm::vec3>    nowRootSpacePosition(num_joints);
            std::vector<glm::vec3>    lastRootSpacePosition(num_joints);

            Update(nowRatio, true);
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

            Update(lastRatio, true);
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

            return result[0].rootSpaceVelocity;
        }

        std::vector<std::array<float, 7>> GetNowPose();
        std::vector<JointFeature>         GetNowJointFeature(float deltaTime);
    };
} // namespace Engine