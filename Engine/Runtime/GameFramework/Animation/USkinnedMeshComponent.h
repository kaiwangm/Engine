#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/GameFramework/Animation/USkeletonComponent.h>
#include <Engine/Runtime/Renderer/VertexArray.h>

#include <Engine/Runtime/GameFramework/Material/MBasicPbr.h>

#include <Engine/Runtime/GameFramework/Animation/ozz/mesh.h>
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
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        std::vector<Ref<IndexBuffer>>  m_IndexBuffers;
        std::vector<Ref<VertexArray>>  m_VertexArrays;

        float m_FrameTime;

        MMaterial* m_Material;

    private:
        static bool loadMesh(const std::string filename, ozz::vector<ozz::sample::Mesh>* _meshes);
        void        prepareMesh();
        bool        drawSkinnedMesh(const ozz::sample::Mesh&              _mesh,
                                    const ozz::span<ozz::math::Float4x4>& _skinning_matrices,
                                    const int                             index);

    public:
        USkinnedMeshComponent();

        USkinnedMeshComponent(const std::string& skeletonPath,
                              const std::string& animationPath,
                              const std::string& meshPath);

    public:
        void Update(float ratio);
        void DrawSkeleton(Ref<Shader> skeletonShader,
                          glm::mat4   vpMat,
                          glm::mat4   transform,
                          glm::vec3   viewPos,
                          glm::vec3   color,
                          glm::vec3   lightColor);
        void DrawSkinnedMesh(Ref<Shader> skinnedMeshShader, glm::mat4 transform, glm::mat4 projection, glm::mat4 view);

    public:
        USkeletonComponent&  GetSkeletonComponentRef() { return m_Skeleton; }
        MMaterial*           GetMaterial() { return m_Material; }
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

    public:
        glm::vec3 GetNowRootPosition() const { return m_Skeleton.GetNowRootPosition(); }
        glm::quat GetNowRootOrientation() const { return m_Skeleton.GetNowRootOrientation(); }
    };
} // namespace Engine