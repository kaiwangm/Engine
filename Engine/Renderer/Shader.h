#pragma once

#include "Core.h"

namespace Engine {
class Shader {
   public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual void SetInt(const std::string& name, const int& value) = 0;
    virtual void SetIntArray(const std::string& name, int* values,
                             uint32_t count) = 0;
    virtual void SetFloat(const std::string& name, const float& value) = 0;
    virtual void SetFloat2(const std::string& name,
                           const glm::vec2& vector) = 0;
    virtual void SetFloat3(const std::string& name,
                           const glm::vec3& vector) = 0;
    virtual void SetFloat4(const std::string& name,
                           const glm::vec4& vector) = 0;
    virtual void SetMat2(const std::string& name, const glm::mat2& matrix) = 0;
    virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

    static Ref<Shader> Create(const std::string& vertexSrc,
                              const std::string& fragmentSrc);
};
}  // namespace Engine