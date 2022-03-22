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

    void LoadShader(const std::string& name, const std::string& vertexSrc,
                    const std::string& fragmentSrc, const std::string& mode);

    void TickLogic(float timeStep, float nowTime);

    void TickRender(float timeStep, float nowTime);

   public:
    ShaderLibrary m_ShaderLibrary;

    Ref<FrameRenderBuffer> m_FrameRenderBuffer;
    Ref<FrameRenderBuffer> m_FrameRenderBuffer_normal;

    glm::vec4 backGroundColor{0.7f, 0.7f, 0.7f, 1.0f};

   private:
    entt::registry m_Registry;

    friend class Entity;
};
}  // namespace Engine