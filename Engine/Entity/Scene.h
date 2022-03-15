#pragma once

#include <entt/entt.hpp>

#include "Components.h"
#include "Core.h"

namespace Engine {
class Entity;

class Scene {
   public:
    Scene();
    ~Scene();

    static Entity CreateEntity(const Ref<Scene> scene,
                               const std::string& name = std::string());
    static void DestroyEntity(const Ref<Scene> scene, Entity entity);

    void OnUpdateRuntime(float timeStep);

    void OnUpdateRuntimeGui(float timeStep, float nowTime);

   private:
    entt::registry m_Registry;

    friend class Entity;
};
}  // namespace Engine