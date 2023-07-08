#pragma once
#include <Engine/Runtime/Core/Core.h>

#include <entt/entt.hpp>
#include <vector>

#include <Engine/Runtime/GameFramework/Common/UObject.h>
#include <Engine/Runtime/GameFramework/Camera/UCameraComponent.h>
#include <Engine/Runtime/Renderer/Renderer.h>

namespace Engine
{
    class AActor;
    class ACamera;
    class ASkybox;

    class UWorld : public UObject
    {
    private:
        std::vector<Ref<AActor>> m_Actors;
        entt::registry           m_Registry;
        ShaderLibrary            m_ShaderLibrary;

    public:
        Ref<FrameRenderBuffer> m_FrameRenderBuffer;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_bufferViewport;
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_playground;

        Ref<GeometryBuffer> m_GeometryBuffer;
        Ref<SSAOBuffer>     m_SSAOBuffer;

        glm::vec4 m_BackGroundColor {0.7f, 0.7f, 0.7f, 1.0f};

        UCameraComponent* m_MainCamera;
        AActor*  m_ControlledActor;
        ASkybox* m_MainSkybox;

        glm::mat4 m_VMatrix;
        glm::mat4 m_PMatrix;
        glm::mat4 m_VPMatrix;

        float m_Exposure                = 0.66f;
        float m_VisPrePrefilterMipLevel = 0.0f;
        int   m_ViewportGBufferMap      = 0;

        entt::entity entity_selected = entt::null;

    public:
        UWorld();

        template<class T, class... Args>
        T& AddActor(const std::string& name, Args... args)
        {
            Ref<T> actor = std::make_shared<T>(this, m_Registry.create(), name, args...);
            m_Actors.push_back(actor);

            return *actor;
        }

        template<class T, class... Args>
        T* RegisterComponents(entt::entity handle, Args... args)
        {
            T& prt = m_Registry.emplace<T>(handle, args...);

            return &prt;
        }

        void LoadShader(const std::string& name,
                        const std::string& vertexSrc,
                        const std::string& fragmentSrc,
                        const std::string& mode)
        {
            m_ShaderLibrary.Load(name, vertexSrc, fragmentSrc, mode);
        }

        void TickLogic(float timeStep, float nowTime, bool isWindowFocused);
        void TickRender(float timeStep);
        void TickGui(float timeStep);

        entt::registry& GetRegistry() { return m_Registry; }
    };
}; // namespace Engine