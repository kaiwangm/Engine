#include <Engine/Runtime/GameFramework/Animation/USkinnedMeshComponent.h>
#include <Engine/Runtime/Renderer/Renderer.h>
#include <ozz/base/io/archive.h>
#include <ozz/base/io/stream.h>

#include <ozz/base/memory/allocator.h>
#include <ozz/geometry/runtime/skinning_job.h>
#include <vector>

namespace Engine
{
    ScratchBuffer::ScratchBuffer() : buffer_(nullptr), size_(0) {}

    ScratchBuffer::~ScratchBuffer() { ozz::memory::default_allocator()->Deallocate(buffer_); }

    void* ScratchBuffer::Resize(size_t _size)
    {
        if (_size > size_)
        {
            size_ = _size;
            ozz::memory::default_allocator()->Deallocate(buffer_);
            buffer_ = ozz::memory::default_allocator()->Allocate(_size, 16);
        }
        return buffer_;
    }

    bool USkinnedMeshComponent::loadMesh(const std::string filename, ozz::vector<ozz::sample::Mesh>* _meshes)
    {
        ozz::io::File file(filename.c_str(), "rb");
        if (!file.opened())
        {
            return false;
        }
        ozz::io::IArchive archive(&file);

        while (archive.TestTag<ozz::sample::Mesh>())
        {
            _meshes->resize(_meshes->size() + 1);
            archive >> _meshes->back();
        }
        return true;
    }

    void USkinnedMeshComponent::prepareMesh()
    {
        for (const ozz::sample::Mesh& mesh : meshes)
        {
            num_skinning_matrices = ozz::math::Max(num_skinning_matrices, mesh.joint_remaps.size());
        }

        skinning_matrices.resize(num_skinning_matrices);

        for (const ozz::sample::Mesh& mesh : meshes)
        {
            if (m_Skeleton.GetNumJoints() < mesh.highest_joint_index())
            {
                Log::Error("Mesh has more joints than skeleton.");
            }
        }

        m_NumParts = meshes[0].parts.size();

        m_VertexBuffers.resize(m_NumParts);
        m_IndexBuffers.resize(m_NumParts);
        m_VertexArrays.resize(m_NumParts);
        for (int i = 0; i < m_VertexArrays.size(); i++)
        {
            m_VertexBuffers[i]  = VertexBuffer::Create();
            BufferLayout layout = {
                {0, ShaderDataType::Float3, "a_Position"},
                {1, ShaderDataType::Float3, "a_Normal"},
                {2, ShaderDataType::Float2, "a_TexCoord"},
                {3, ShaderDataType::Float3, "a_Tangent"},
            };
            m_VertexBuffers[i]->SetLayout(layout);

            m_IndexBuffers[i] = IndexBuffer::Create();

            m_VertexArrays[i] = VertexArray::Create();
            m_VertexArrays[i]->AddVertexBuffer(m_VertexBuffers[i], false);
            m_VertexArrays[i]->AddIndexBuffer(m_IndexBuffers[i]);
        }
    }

    bool USkinnedMeshComponent::updateSkinnedMesh(const ozz::sample::Mesh&              _mesh,
                                                  const ozz::span<ozz::math::Float4x4>& _skinning_matrices,
                                                  const int                             index)
    {
        // Renders skin.

        const int        vertex_count = _mesh.vertex_count();
        std::vector<int> partition_vertices_start(_mesh.parts.size());
        std::vector<int> partition_vertices_count(_mesh.parts.size());

        // Positions and normals are interleaved to improve caching while executing
        // skinning job.

        const GLsizei positions_offset  = 0;
        const GLsizei positions_stride  = sizeof(float) * 3;
        const GLsizei normals_offset    = vertex_count * positions_stride;
        const GLsizei normals_stride    = sizeof(float) * 3;
        const GLsizei tangents_offset   = normals_offset + vertex_count * normals_stride;
        const GLsizei tangents_stride   = sizeof(float) * 3;
        const GLsizei skinned_data_size = tangents_offset + vertex_count * tangents_stride;

        // Colors and uvs are contiguous. They aren't transformed, so they can be
        // directly copied from source mesh which is non-interleaved as-well.
        // Colors will be filled with white if _options.colors is false.
        // UVs will be skipped if _options.textured is false.
        const GLsizei colors_offset   = skinned_data_size;
        const GLsizei colors_stride   = sizeof(uint8_t) * 4;
        const GLsizei colors_size     = vertex_count * colors_stride;
        const GLsizei uvs_offset      = colors_offset + colors_size;
        const GLsizei uvs_stride      = true ? sizeof(float) * 2 : 0;
        const GLsizei uvs_size        = vertex_count * uvs_stride;
        const GLsizei fixed_data_size = colors_size + uvs_size;

        // Reallocate vertex buffer.
        const GLsizei vbo_size = skinned_data_size + fixed_data_size;
        void*         vbo_map  = scratch_buffer_.Resize(vbo_size);

        // Iterate mesh parts and fills vbo.
        // Runs a skinning job per mesh part. Triangle indices are shared
        // across parts.
        size_t processed_vertex_count = 0;
        for (size_t i = 0; i < _mesh.parts.size(); ++i)
        {
            const ozz::sample::Mesh::Part& part = _mesh.parts[i];

            // Skip this iteration if no vertex.
            const size_t part_vertex_count = part.positions.size() / 3;
            if (part_vertex_count == 0)
            {
                continue;
            }

            // Fills the job.
            ozz::geometry::SkinningJob skinning_job;
            skinning_job.vertex_count       = static_cast<int>(part_vertex_count);
            const int part_influences_count = part.influences_count();

            // Clamps joints influence count according to the option.
            skinning_job.influences_count = part_influences_count;

            // Setup skinning matrices, that came from the animation stage before being
            // multiplied by inverse model-space bind-pose.
            skinning_job.joint_matrices = _skinning_matrices;

            // Setup joint's indices.
            skinning_job.joint_indices        = make_span(part.joint_indices);
            skinning_job.joint_indices_stride = sizeof(uint16_t) * part_influences_count;

            // Setup joint's weights.
            if (part_influences_count > 1)
            {
                skinning_job.joint_weights        = make_span(part.joint_weights);
                skinning_job.joint_weights_stride = sizeof(float) * (part_influences_count - 1);
            }

            // Setup input positions, coming from the loaded mesh.
            skinning_job.in_positions        = make_span(part.positions);
            skinning_job.in_positions_stride = sizeof(float) * ozz::sample::Mesh::Part::kPositionsCpnts;

            // Setup output positions, coming from the rendering output mesh buffers.
            // We need to offset the buffer every loop.
            float* out_positions_begin = reinterpret_cast<float*>(
                ozz::PointerStride(vbo_map, positions_offset + processed_vertex_count * positions_stride));
            float* out_positions_end   = ozz::PointerStride(out_positions_begin, part_vertex_count * positions_stride);
            skinning_job.out_positions = {out_positions_begin, out_positions_end};
            skinning_job.out_positions_stride = positions_stride;

            // Setup normals if input are provided.
            float* out_normal_begin = reinterpret_cast<float*>(
                ozz::PointerStride(vbo_map, normals_offset + processed_vertex_count * normals_stride));
            float* out_normal_end = ozz::PointerStride(out_normal_begin, part_vertex_count * normals_stride);

            if (part.normals.size() / ozz::sample::Mesh::Part::kNormalsCpnts == part_vertex_count)
            {
                // Setup input normals, coming from the loaded mesh.
                skinning_job.in_normals        = make_span(part.normals);
                skinning_job.in_normals_stride = sizeof(float) * ozz::sample::Mesh::Part::kNormalsCpnts;

                // Setup output normals, coming from the rendering output mesh buffers.
                // We need to offset the buffer every loop.
                skinning_job.out_normals        = {out_normal_begin, out_normal_end};
                skinning_job.out_normals_stride = normals_stride;
            }
            else
            {
                // Fills output with default normals.
                for (float* normal = out_normal_begin; normal < out_normal_end;
                     normal        = ozz::PointerStride(normal, normals_stride))
                {
                    normal[0] = 0.f;
                    normal[1] = 1.f;
                    normal[2] = 0.f;
                }
            }

            // Setup tangents if input are provided.
            float* out_tangent_begin = reinterpret_cast<float*>(
                ozz::PointerStride(vbo_map, tangents_offset + processed_vertex_count * tangents_stride));
            float* out_tangent_end = ozz::PointerStride(out_tangent_begin, part_vertex_count * tangents_stride);

            if (part.tangents.size() / ozz::sample::Mesh::Part::kTangentsCpnts == part_vertex_count)
            {
                // Setup input tangents, coming from the loaded mesh.
                skinning_job.in_tangents        = make_span(part.tangents);
                skinning_job.in_tangents_stride = sizeof(float) * ozz::sample::Mesh::Part::kTangentsCpnts;

                // Setup output tangents, coming from the rendering output mesh buffers.
                // We need to offset the buffer every loop.
                skinning_job.out_tangents        = {out_tangent_begin, out_tangent_end};
                skinning_job.out_tangents_stride = tangents_stride;
            }
            else
            {
                // Fills output with default tangents.
                for (float* tangent = out_tangent_begin; tangent < out_tangent_end;
                     tangent        = ozz::PointerStride(tangent, tangents_stride))
                {
                    tangent[0] = 1.f;
                    tangent[1] = 0.f;
                    tangent[2] = 0.f;
                }
            }

            // Execute the job, which should succeed unless a parameter is invalid.
            if (!skinning_job.Run())
            {
                return false;
            }

            // fill uvs
            memcpy(ozz::PointerStride(vbo_map, uvs_offset + processed_vertex_count * uvs_stride),
                   array_begin(part.uvs),
                   part_vertex_count * uvs_stride);

            partition_vertices_start[i] = static_cast<int>(processed_vertex_count);
            partition_vertices_count[i] = static_cast<int>(part_vertex_count);

            // Some more vertices were processed.
            processed_vertex_count += part_vertex_count;
        }

        std::vector<float> soa(vertex_count * 11);
        float* out_positions_begin = reinterpret_cast<float*>(ozz::PointerStride(vbo_map, positions_offset));
        float* out_normal_begin    = reinterpret_cast<float*>(ozz::PointerStride(vbo_map, normals_offset));
        float* out_texcoords_begin = reinterpret_cast<float*>(ozz::PointerStride(vbo_map, uvs_offset));
        float* out_tangent_begin   = reinterpret_cast<float*>(ozz::PointerStride(vbo_map, tangents_offset));

        for (int idx = 0; idx < vertex_count; idx++)
        {
            soa[idx * 11 + 0]  = out_positions_begin[idx * 3 + 0];
            soa[idx * 11 + 1]  = out_positions_begin[idx * 3 + 1];
            soa[idx * 11 + 2]  = out_positions_begin[idx * 3 + 2];
            soa[idx * 11 + 3]  = out_normal_begin[idx * 3 + 0];
            soa[idx * 11 + 4]  = out_normal_begin[idx * 3 + 1];
            soa[idx * 11 + 5]  = out_normal_begin[idx * 3 + 2];
            soa[idx * 11 + 6]  = out_texcoords_begin[idx * 2 + 0];
            soa[idx * 11 + 7]  = 1.0f - out_texcoords_begin[idx * 2 + 1];
            soa[idx * 11 + 8]  = out_tangent_begin[idx * 3 + 0];
            soa[idx * 11 + 9]  = out_tangent_begin[idx * 3 + 1];
            soa[idx * 11 + 10] = out_tangent_begin[idx * 3 + 2];
        }

        const ozz::vector<uint16_t>& indices = _mesh.triangle_indices;
        std::vector<uint32_t>        indices32(indices.begin(), indices.end());

        for (int i = 0; i < m_NumParts; ++i)
        {
            const GLsizei vertices_start = partition_vertices_start[i];
            const GLsizei vertices_count = partition_vertices_count[i];
            const GLsizei soa_size       = vertex_count * 11 * sizeof(float);

            m_VertexBuffers[i]->Bind();
            glBufferData(GL_ARRAY_BUFFER, soa_size, soa.data(), GL_DYNAMIC_DRAW);
            m_VertexBuffers[i]->UnBind();

            std::vector<uint32_t> partition_indices;
            uint32_t              partition_indices_start = m_TexturesMetainfo[i].start;
            uint32_t              partition_indices_count = m_TexturesMetainfo[i].count;

            for (int j = partition_indices_start * 3; j < (partition_indices_start + partition_indices_count) * 3; j++)
            {
                partition_indices.push_back(indices32[j]);
            }

            m_IndexBuffers[i]->Bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         partition_indices.size() * sizeof(uint32_t),
                         partition_indices.data(),
                         GL_DYNAMIC_DRAW);
            m_IndexBuffers[i]->SetCount(partition_indices.size());
            m_IndexBuffers[i]->UnBind();
        }

        return true;
    }

    USkinnedMeshComponent::USkinnedMeshComponent() {}

    USkinnedMeshComponent::USkinnedMeshComponent(const std::string& skeletonPath,
                                                 const std::string& animationPath,
                                                 const std::string& meshPath) :
        m_Skeleton(skeletonPath, animationPath)
    {
        loadMesh(meshPath, &meshes);
        prepareMesh();
        m_Skeleton.SetShowSkeleton(false);

        m_TexturesMetainfo = {
            {"Assets/Editor/Animation/silverwolf/texture/hair.png", 5443, 0},
            {"Assets/Editor/Animation/silverwolf/texture/cloth.png", 13019, 0},
            {"Assets/Editor/Animation/silverwolf/texture/jacket.png", 8619, 0},
            {"Assets/Editor/Animation/silverwolf/texture/jacket.png", 281, 0},
            {"Assets/Editor/Animation/silverwolf/texture/face.png", 4210, 0},
            {"Assets/Editor/Animation/silverwolf/texture/face.png", 56, 0},
            {"Assets/Editor/Animation/silverwolf/texture/emoticon.png", 442, 0},
        };

        uint32_t start = 0;
        for (int i = 0; i < meshes[0].parts.size(); ++i)
        {
            m_TexturesMetainfo[i].start = start;
            start += m_TexturesMetainfo[i].count;
        }

        for (int i = 0; i < meshes[0].parts.size(); ++i)
        {
            MBasicPbr* material = new MBasicPbr(fmt::format("BasicPbr_{}", i).c_str());

            material->SetAlbedo(glm::vec3(0.9f, 0.9f, 0.9f));
            material->SetMetallic(0.03f);
            material->SetRoughness(0.96f);

            material->LoadAlbedoMap(m_TexturesMetainfo[i].albedoPath);
            material->BufferTextures();

            m_Materials.push_back(material);
        }
    }

    void USkinnedMeshComponent::Update(float ratio) { m_Skeleton.Update(ratio); }

    void USkinnedMeshComponent::DrawSkeleton(Ref<Shader> skeletonShader,
                                             glm::mat4   vpMat,
                                             glm::mat4   transform,
                                             glm::vec3   viewPos,
                                             glm::vec3   color,
                                             glm::vec3   lightColor)
    {
        if (m_Skeleton.GetShowSkeleton())
        {
            m_Skeleton.Draw(skeletonShader, vpMat, transform, viewPos, color, lightColor);
        }
    }

    void USkinnedMeshComponent::DrawSkinnedMesh(Ref<Shader> skinnedMeshShader,
                                                glm::mat4   transform,
                                                glm::mat4   projection,
                                                glm::mat4   view)
    {
        ozz::vector<ozz::math::Float4x4>& models = m_Skeleton.GetModelsRef();

        for (const ozz::sample::Mesh& mesh : meshes)
        {
            for (size_t i = 0; i < mesh.joint_remaps.size(); ++i)
            {
                skinning_matrices[i] = models[mesh.joint_remaps[i]] * mesh.inverse_bind_poses[i];
            }

            auto trans = ozz::math::Float4x4::identity();
            // Renders skin.
            updateSkinnedMesh(mesh, make_span(skinning_matrices), 0);

            skinnedMeshShader->SetMat4("u_MProjection", projection);
            skinnedMeshShader->SetMat4("u_MView", view);
            skinnedMeshShader->SetMat4("u_MTransform", transform);

            for (int i = 0; i < m_NumParts; ++i)
            {
                MBasicPbr* material = dynamic_cast<MBasicPbr*>(m_Materials[i]);
                material->BindAllMap(skinnedMeshShader);

                m_VertexArrays[i]->Bind();

                RenderCommand::DrawIndexed(m_VertexArrays[i]);

                m_VertexArrays[i]->UnBind();

                material->UnBindAllMap(skinnedMeshShader);
            }
        }
    }

} // namespace Engine