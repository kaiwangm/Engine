#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>

#include <Engine/Runtime/GameFramework/Common/UObject.h>

namespace Engine
{
    class UComponent : public UObject
    {
    protected:
        UObject* m_Owner;

    public:
        UComponent(UObject* owner) : m_Owner(owner) {}
        UComponent() {}
        virtual ~UComponent() { m_Owner = nullptr; }

        void     SetOwner(UObject* owner) { m_Owner = owner; }
        UObject* GetOwner() const { return m_Owner; }

        virtual void Tick(float deltaTime) {};
        virtual void Destory() {};
    };

    class UTagComponent final : public UComponent
    {
    private:
        std::string m_Tag;

    public:
        UTagComponent()                     = default;
        UTagComponent(const UTagComponent&) = default;
        UTagComponent(const std::string& tag) : m_Tag(tag) {}

        std::string GetString() const { return m_Tag; }
        void        SetString(const std::string& tag) { m_Tag = tag; }
    };

    class UTransformComponent final : public UComponent
    {
    private:
        glm::vec3 m_Position;
        glm::vec3 m_Rotation;
        glm::vec3 m_Scale;

    public:
        UTransformComponent(const UTransformComponent&) = default;
        UTransformComponent(const glm::vec3& position = glm::vec3 {0.0f, 0.0f, 0.0f},
                            const glm::vec3& rotation = glm::vec3 {0.0f, 0.0f, 0.0f},
                            const glm::vec3& scale    = glm::vec3 {1.0f, 1.0f, 1.0f}) :
            m_Position(position),
            m_Rotation(rotation), m_Scale(scale)
        {}

        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

            return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
        }

        glm::vec3 GetPosition() const { return m_Position; }
        glm::vec3 GetRotation() const { return m_Rotation; }
        glm::vec3 GetScale() const { return m_Scale; }

        glm::vec3& GetPositionRef() { return m_Position; }
        glm::vec3& GetRotationRef() { return m_Rotation; }
        glm::vec3& GetScaleRef() { return m_Scale; }

        void SetPosition(const glm::vec3& position) { m_Position = position; }
        void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
        void SetScale(const glm::vec3& scale) { m_Scale = scale; }
    };
}; // namespace Engine