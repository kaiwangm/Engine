#include "Entity.h"

namespace Engine {
Entity::Entity(entt::entity handle, WeakRef<Scene> scene)
    : m_EntityHandle(handle), m_Scene(scene) {}

}  // namespace Engine