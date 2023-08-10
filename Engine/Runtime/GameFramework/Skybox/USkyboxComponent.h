#pragma once
#include <Engine/Runtime/Renderer/Texture.h>
#include <Engine/Runtime/GameFramework/DataUtil/Mesh.h>
#include <Engine/Runtime/GameFramework/Common/UComponent.h>

namespace Engine
{
    class USkyboxComponent final : public UComponent
    {
    private:
        Ref<CubeMap> m_CubeMap;
        Ref<Mesh>    m_Skybox;

    public:
        USkyboxComponent(const std::string path)
        {
            m_CubeMap             = CubeMap::Create(path);
            float skyVertices[24] = {
                -1.0f, -1.0f, -1.0f, //
                1.0f,  -1.0f, -1.0f, //
                -1.0f, 1.0f,  -1.0f, //
                1.0f,  1.0f,  -1.0f, //
                -1.0f, -1.0f, 1.0f,  //
                1.0f,  -1.0f, 1.0f,  //
                -1.0f, 1.0f,  1.0f,  //
                1.0f,  1.0f,  1.0f   //
            };

            uint32_t skyIndices[36] = {
                1, 5, 7, 1, 7, 3, // positive x
                2, 0, 4, 2, 4, 6, // negative x
                4, 5, 7, 4, 7, 6, // positive y
                0, 1, 3, 0, 3, 2, // negative y
                0, 1, 5, 0, 5, 4, // positive z
                3, 2, 6, 3, 6, 7  // negative z
            };

            const BufferLayout layout = BufferLayout {
                {0, ShaderDataType::Float3, "a_Position"},
            };

            m_Skybox = std::make_shared<Mesh>(skyVertices, skyIndices, 8, 3, 36, layout);
        }

        void Tick(float deltaTime) override {}
        void Draw(const Ref<Shader>& shader, const glm::mat4 vpmat)
        {
            Renderer::SetShaderUniform(shader, "environmentMap", 0);

            m_CubeMap->Bind(0);
            shader->Bind();
            shader->SetMat4("u_ViewProjection", vpmat);

            auto& vertexArray = m_Skybox->m_VertexArray; 

            vertexArray->Bind();
            RenderCommand::DrawSkybox(vertexArray);

            vertexArray->UnBind();
            shader->UnBind();
            m_CubeMap->UnBind(0);
        }

        CubeMap* GetCubeMap() { return m_CubeMap.get(); }
        Mesh&    GetSkyboxRef() { return *m_Skybox; }
    };
}; // namespace Engine