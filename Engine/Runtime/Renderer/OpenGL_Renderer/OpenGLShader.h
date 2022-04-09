#pragma once
#include "Core.h"
#include "../Shader.h"

namespace Engine {
class OpenGLShader : public Shader {
   public:
    OpenGLShader(const std::string& name, const std::string& vertexSrc,
                 const std::string& fragmentSrc, const std::string& mode);

    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void SetInt(const std::string& name, const int& value) override;
    virtual void SetIntArray(const std::string& name, int* values,
                             uint32_t count) override;
    virtual void SetFloat(const std::string& name, const float& value) override;
    virtual void SetFloat2(const std::string& name,
                           const glm::vec2& vector) override;
    virtual void SetFloat3(const std::string& name,
                           const glm::vec3& vector) override;
    virtual void SetFloat4(const std::string& name,
                           const glm::vec4& vector) override;
    virtual void SetMat2(const std::string& name,
                         const glm::mat2& matrix) override;
    virtual void SetMat3(const std::string& name,
                         const glm::mat3& matrix) override;
    virtual void SetMat4(const std::string& name,
                         const glm::mat4& matrix) override;

   private:
    void Compile(const std::string& vertexShaderSource,
                 const std::string& fragmentShaderSource);
    std::string ReadFile(const std::string& shaderPath);

    GLuint m_RendererID;
};
}  // namespace Engine