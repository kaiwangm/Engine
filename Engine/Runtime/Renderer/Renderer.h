#pragma once

#include "Core.h"
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Engine
{
    class Camera;
    class Renderer
    {
    public:
        static void Init();
        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(const Ref<Camera>& camera);
        static void BeginScene(const Ref<Camera>& camera, const Ref<FrameRenderBuffer> framerenderbuffer);
        static void EndScene();
        static void EndScene(const Ref<FrameRenderBuffer> framerenderbuffer);

        static void BindShader(const Ref<Shader>& shader) { shader->Bind(); }
        static void UnBindShader(const Ref<Shader>& shader) { shader->UnBind(); }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, const int& value)
        {
            shader->Bind();
            shader->SetInt(name, value);
            shader->UnBind();
        }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, int* values, uint32_t count)
        {
            shader->Bind();
            shader->SetIntArray(name, values, count);
            shader->UnBind();
        }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, const float& value)
        {
            shader->Bind();
            shader->SetFloat(name, value);
            shader->UnBind();
        }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, const glm::vec2& vector)
        {
            shader->Bind();
            shader->SetFloat2(name, vector);
            shader->UnBind();
        }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, const glm::vec3& vector)
        {
            shader->Bind();
            shader->SetFloat3(name, vector);
            shader->UnBind();
        }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, const glm::vec4& vector)
        {
            shader->Bind();
            shader->SetFloat4(name, vector);
            shader->UnBind();
        }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, const glm::mat2& matrix)
        {
            shader->Bind();
            shader->SetMat2(name, matrix);
            shader->UnBind();
        }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, const glm::mat3& matrix)
        {
            shader->Bind();
            shader->SetMat3(name, matrix);
            shader->UnBind();
        }
        static void SetShaderUniform(const Ref<Shader>& shader, const std::string& name, const glm::mat4& matrix)
        {
            shader->Bind();
            shader->SetMat4(name, matrix);
            shader->UnBind();
        }

        static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform);

        static void Submit(const Ref<VertexArray>& vertexArray,
                           const Ref<Shader>&      shader,
                           const glm::mat4&        pvMatrix,
                           const glm::mat4&        transform);

        static void
        DrawArray(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform);

        static void
        DrawSkybox(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& pvMatrix);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> s_SceneData;
    };
} // namespace Engine