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

struct StaticModelComponent {
    WeakRef<Model> m_Model;

    StaticModelComponent(const Ref<Model> model) : m_Model(model) {}

    Ref<Model> GetModel() { return m_Model.lock(); }
};

struct CameraComponent {
    WeakRef<Camera> m_Camera;

    CameraComponent(const Ref<Camera> camera) : m_Camera(camera) {}

    Ref<Camera> GetCamera() { return m_Camera.lock(); }
};
}  // namespace Engine