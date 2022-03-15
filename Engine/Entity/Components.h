#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Model.h"

namespace Engine {
struct TagComponent {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag) : Tag(tag) {}

    std::string GetString() { return Tag; }
};

struct TransformComponent {
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation)
        : Translation(translation) {}

    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

        return glm::translate(glm::mat4(1.0f), Translation) * rotation *
               glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct RenderableMeshComponent {
    WeakRef<VertexArray> m_VertexArray;
    WeakRef<Shader> m_Shader;
    WeakRef<Texture2D> m_Texture;

    RenderableMeshComponent() = default;

    RenderableMeshComponent(const Ref<VertexArray> vertexArray,
                            const Ref<Shader> shader,
                            const Ref<Texture2D> texture)
        : m_VertexArray(vertexArray), m_Shader(shader), m_Texture(texture) {}

    Ref<VertexArray> GetVertexArray() { return m_VertexArray.lock(); }
    Ref<Shader> GetShader() { return m_Shader.lock(); }
    Ref<Texture2D> GetTexture() { return m_Texture.lock(); }
};

struct StaticModelComponent {
    WeakRef<Model> m_Model;

    StaticModelComponent(const Ref<Model> model) : m_Model(model) {}

    Ref<Model> GetModel() { return m_Model.lock(); }
};
}  // namespace Engine