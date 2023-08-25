#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/GameFramework/Animation/USkeletonComponent.h>
#include <Engine/Runtime/Renderer/VertexArray.h>

#include <Engine/Runtime/GameFramework/Material/MBasicPbr.h>

#include <Engine/Runtime/GameFramework/Animation/ozz/mesh.h>

#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/base/containers/vector.h>
#include <ozz/base/maths/math_ex.h>

namespace Engine
{
    class ScratchBuffer
    {
    public:
        ScratchBuffer();
        ~ScratchBuffer();

        // Resizes the buffer to the new size and return the memory address.
        void* Resize(size_t _size);

    private:
        void*  buffer_;
        size_t size_;
    };

    class USkinnedMeshComponent : public UComponent
    {
    private:
        UTransformComponent m_Transform;
        USkeletonComponent  m_Skeleton;

    private:
        ozz::vector<ozz::sample::Mesh>   meshes;
        size_t                           num_skinning_matrices = 0;
        ozz::vector<ozz::math::Float4x4> skinning_matrices;

        ScratchBuffer                  scratch_buffer_;
        uint32_t                       m_NumParts = 0;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        std::vector<Ref<IndexBuffer>>  m_IndexBuffers;
        std::vector<Ref<VertexArray>>  m_VertexArrays;

        std::vector<MMaterial*> m_Materials;

        struct TextureMetainfo
        {
            std::string albedoPath;
            uint32_t    count;
            uint32_t    start;
        };

        std::vector<TextureMetainfo> m_TexturesMetainfo;

    private:
        static bool loadMesh(const std::string filename, ozz::vector<ozz::sample::Mesh>* _meshes);
        void        prepareMesh();
        bool        updateSkinnedMesh(const ozz::sample::Mesh&              _mesh,
                                      const ozz::span<ozz::math::Float4x4>& _skinning_matrices,
                                      const int                             index);

    public:
        USkinnedMeshComponent();

        USkinnedMeshComponent(const std::string& skeletonPath,
                              const std::string& animationPath,
                              const std::string& meshPath);

    public:
        void Update(float ratio);
        bool GetLocals(ozz::vector<ozz::math::SoaTransform>& inLocals, float ratio)
        {
            return m_Skeleton.GetLocals(inLocals, ratio);
        }
        void DrawSkeleton(Ref<Shader> skeletonShader,
                          glm::mat4   vpMat,
                          glm::mat4   transform,
                          glm::vec3   viewPos,
                          glm::vec3   color,
                          glm::vec3   lightColor);
        void DrawSkinnedMesh(Ref<Shader> skinnedMeshShader, glm::mat4 transform, glm::mat4 projection, glm::mat4 view);

    public:
        USkeletonComponent&  GetSkeletonComponentRef() { return m_Skeleton; }
        MMaterial*           GetMaterial(int index) { return m_Materials[index]; }
        UTransformComponent& GetTransformComponentRef() { return m_Transform; }
        size_t               GetNumSkinningMatrices() const { return num_skinning_matrices; }
        float                GetFrameTime() const { return m_Skeleton.GetFrameTime(); }

    public:
        bool m_ShowSkinnedMesh = true;

    public:
        bool  GetShowSkinnedMesh() const { return m_ShowSkinnedMesh; }
        bool& GetShowSkinnedMeshRef() { return m_ShowSkinnedMesh; }
        void  SetShowSkinnedMesh(bool showSkinnedMesh) { m_ShowSkinnedMesh = showSkinnedMesh; }
        bool  GetUseRootMotion() const { return m_Skeleton.GetUseRootMotion(); }
        bool& GetUseRootMotionRef() { return m_Skeleton.GetUseRootMotionRef(); }
        void  SetUseRootMotion(bool useRootMotion) { m_Skeleton.SetUseRootMotion(useRootMotion); }

        void SetModels(const ozz::vector<ozz::math::Float4x4>& models) { m_Skeleton.SetModels(models); }
        ozz::vector<ozz::math::Float4x4> GetModels() const { return m_Skeleton.GetModels(); }

        glm::vec3                         GetNowRootPosition() const { return m_Skeleton.GetNowRootPosition(); }
        glm::quat                         GetNowRootOrientation() const { return m_Skeleton.GetNowRootOrientation(); }
        std::vector<std::array<float, 7>> GetNowPose() { return m_Skeleton.GetNowPose(); }

        std::vector<JointFeature> GetNowJointFeature(float deltaTime)
        {
            return m_Skeleton.GetNowJointFeature(deltaTime);
        }

        glm::vec3 GetVelocity(float deltaTime) { return m_Skeleton.GetVelocity(deltaTime); }

        ozz::animation::Skeleton& GetSkeletonRef() { return m_Skeleton.GetSkeletonRef(); }
    };
} // namespace Engine