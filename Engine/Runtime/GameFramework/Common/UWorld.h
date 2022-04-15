#pragma once
#include "Core.h"

#include <entt/entt.hpp>
#include <vector>

#include "UObject.h"
#include "Renderer.h"


namespace Engine {
class AActor;

class UWorld : public UObject {
   private:
    std::vector<Ref<AActor>> m_Actors;
    entt::registry m_Registry;
    ShaderLibrary m_ShaderLibrary;

   public:
    Ref<FrameRenderBuffer> m_FrameRenderBuffer;
    Ref<FrameRenderBuffer> m_FrameRenderBuffer_normal;
    glm::vec4 backGroundColor{0.7f, 0.7f, 0.7f, 1.0f};

   public:
    UWorld();

    template <class T, class... Args>
    T& AddActor(const std::string& name, Args... args) {
        Ref<T> actor =
            std::make_shared<T>(this, m_Registry.create(), name, args...);
        m_Actors.push_back(actor);

        return *actor;
    }

    template <class T, class... Args>
    T* RegisterComponents(entt::entity handle, Args... args) {
        T& prt = m_Registry.emplace<T>(handle, args...);

        return &prt;
    }

    void LoadShader(const std::string& name, const std::string& vertexSrc,
                    const std::string& fragmentSrc, const std::string& mode) {
        m_ShaderLibrary.Load(name, vertexSrc, fragmentSrc, mode);
    }

    void OnUpdateRuntime(float timeStep);
    void OnUpdateRuntimeGui(float timeStep, float nowTime);
    void TickLogic(float timeStep, float nowTime, bool handleInput);
    void TickRender(float timeStep, float nowTime);
};
};  // namespace Engine