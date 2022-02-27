#pragma once

#include "Core.h"

namespace Engine {
class Shader {
   public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void UnBind() const;

    void SetInt(const std::string& name, const int& value);
    void SetIntArray(const std::string& name, int* values, uint32_t count);
    void SetFloat(const std::string& name, const float& value);
    void SetFloat2(const std::string& name, const glm::vec2& vector);
    void SetFloat3(const std::string& name, const glm::vec3& vector);
    void SetFloat4(const std::string& name, const glm::vec4& vector);
    void SetMat2(const std::string& name, const glm::mat2& matrix);
    void SetMat3(const std::string& name, const glm::mat3& matrix);
    void SetMat4(const std::string& name, const glm::mat4& matrix);

   private:
    GLuint m_RendererID;
};
}  // namespace Engine