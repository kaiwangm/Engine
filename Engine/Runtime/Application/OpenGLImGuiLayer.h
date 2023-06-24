#pragma once
#include "Core.h"
#include "ImGuiLayer.h"

namespace Engine
{
    class OpenGLImGuiLayer : public ImGuiLayer
    {
    public:
        OpenGLImGuiLayer(const std::string& name, const std::string& basedir);
        virtual ~OpenGLImGuiLayer();
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;

        virtual void Begin() override;
        virtual void End() override;

    private:
        void SetDarkThemeColors();

        std::string m_Basedir;
        std::string m_InitFilePath;
        std::string m_FontPath;
    };
} // namespace Engine