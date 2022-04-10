#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Animation.h"
#include "Model.h"

namespace Engine {

class UObject;

class UComponent {
   protected:
    UObject* m_Owner;

   public:
    UComponent(UObject* owner) : m_Owner(owner) {}
    UComponent() {}
    virtual ~UComponent() { m_Owner = nullptr; }

    void SetOwner(UObject* owner) { m_Owner = owner; }
    UObject* GetOwner() const { return m_Owner; }

    virtual void Tick(float deltaTime){};
    virtual void Destory(){};
};

class UTagComponent : public UComponent {
   public:
    std::string Tag;

    UTagComponent() = default;
    UTagComponent(const UTagComponent&) = default;
    UTagComponent(const std::string& tag) : Tag(tag) {}

    std::string GetString() { return Tag; }
};

class UTransformComponent : public UComponent {
   public:
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

    UTransformComponent() = default;
    UTransformComponent(const UTransformComponent&) = default;
    UTransformComponent(const glm::vec3& translation)
        : Translation(translation) {}
    UTransformComponent(const glm::vec3& translation, const glm::vec3& rotation)
        : Translation(translation), Rotation(rotation) {}
    UTransformComponent(const glm::vec3& translation, const glm::vec3& rotation,
                        const glm::vec3& scale)
        : Translation(translation), Rotation(rotation), Scale(scale) {}

    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

        return glm::translate(glm::mat4(1.0f), Translation) * rotation *
               glm::scale(glm::mat4(1.0f), Scale);
    }
};

class UStaticModelComponent : public UComponent {
   public:
    WeakRef<Model> m_Model;

    UStaticModelComponent(const Ref<Model> model) : m_Model(model) {}

    Ref<Model> GetModel() { return m_Model.lock(); }
};

class UAnimatedModelComponent : public UComponent {
   public:
    WeakRef<AnimatedModel> m_Model;

    UAnimatedModelComponent(const Ref<AnimatedModel> model) : m_Model(model) {}

    Ref<AnimatedModel> GetModel() { return m_Model.lock(); }
};

class UCameraComponent : public UComponent {
   public:
    WeakRef<Camera> m_Camera;

    UCameraComponent(const Ref<Camera> camera) : m_Camera(camera) {}

    Ref<Camera> GetCamera() { return m_Camera.lock(); }
};
}  // namespace Engine