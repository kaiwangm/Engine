#pragma once

#include <entt/entt.hpp>

#include "Core.h"
#include "Scene.h"

namespace Engine {
class Entity {
   public:
    Entity() = default;
    Entity(entt::entity handle, WeakRef<Scene> scene);
    Entity(const Entity& other) = default;

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        T& component = (m_Scene.lock())
                           ->m_Registry.emplace<T>(m_EntityHandle,
                                                   std::forward<Args>(args)...);
        // m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T>
    T& GetComponent() {
        return m_Scene.lock()->m_Registry.get<T>(m_EntityHandle);
    }

    template <typename T>
    bool HasComponent() {
        return m_Scene.lock()->m_Registry.has<T>(m_EntityHandle);
    }

    template <typename T>
    void RemoveComponent() {
        m_Scene.lock()->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const { return m_EntityHandle != entt::null; }
    operator entt::entity() const { return m_EntityHandle; }
    operator uint32_t() const { return (uint32_t)m_EntityHandle; }

    bool operator==(const Entity& other) const {
        return m_EntityHandle == other.m_EntityHandle &&
               m_Scene.lock() == other.m_Scene.lock();
    }

    bool operator!=(const Entity& other) const { return !(*this == other); }

   private:
    entt::entity m_EntityHandle{entt::null};
    WeakRef<Scene> m_Scene;
};
}  // namespace Engine