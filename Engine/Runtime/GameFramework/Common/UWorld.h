#pragma once
#include "Core.h"

#include <entt/entt.hpp>
#include <vector>

#include "UObject.h"
#include "Renderer.h"

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
        Ref<FrameRenderBuffer> m_FrameRenderBuffer_normal;
        glm::vec4              m_BackGroundColor {0.7f, 0.7f, 0.7f, 1.0f};

        ACamera* m_MainCamera;
        ASkybox* m_MainSkybox;

        glm::mat4 m_VMatrix;
        glm::mat4 m_PMatrix;
        glm::mat4 m_VPMatrix;

        float m_Exposure = 1.0f;
        float m_VisPrePrefilterMipLevel = 0.0f;

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

        void TickLogic(float timeStep, float nowTime, bool handleInput);
        void TickRender(float timeStep);
        void TickGui(float timeStep);
    };
}; // namespace Engine