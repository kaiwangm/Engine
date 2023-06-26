#pragma once
#include <Engine/Runtime/GameFramework/Material/MMaterial.h>
#include <Engine/Runtime/Renderer/Texture.h>

namespace Engine
{
    class MTriangleShader final : public MMaterial
    {
    private:
        glm::vec3 m_Color;

    public:
        MTriangleShader(const std::string& name) : MMaterial(name, "TriangleShader") {
            m_Color = glm::vec3(1.0f);
        }

        ~MTriangleShader() = default;

        void Bind(const Ref<Shader> shader) {
            shader->SetFloat3("color", m_Color);
        }

        void UnBind(const Ref<Shader> shader) {}

        glm::vec3& GetColorRef() { return m_Color; }
    };
} // namespace Engine