#pragma once

#include <imgui.h>

#include "Core.h"
#include "ImNodes/ImGuiNodes.h"
#include "Renderer.h"

namespace Engine
{
    class GuiCommand
    {
    public:
        template<class... Args>
        static void Begin(const std::string& name, Args... args)
        {
            ImGui::Begin(fmt::format(name, args...).c_str());
        }

        static void End() { ImGui::End(); }

        template<class... Args>
        static void Text(const std::string& text, Args... args)
        {
            // Text("%s", fmt::format(text, args...).c_str())
            ImGui::TextUnformatted(fmt::format(text, args...).c_str());
        }

        template<class... Args>
        static void ColorEdit4(const std::string& text, glm::vec4& color, Args... args)
        {
            ImGui::ColorEdit4(fmt::format(text, args...).c_str(), glm::value_ptr(color));
        }

        template<class... Args>
        static void
        SliderFloat(const std::string& text, float& value, const float& v_min, const float& v_max, Args... args)
        {
            ImGui::SliderFloat(fmt::format(text, args...).c_str(), &value, v_min, v_max);
        }

        template<class... Args>
        static void
        SliderFloat3(const std::string& text, glm::vec3& values, const float& v_min, const float& v_max, Args... args)
        {
            ImGui::SliderFloat3(fmt::format(text, args...).c_str(), glm::value_ptr(values), v_min, v_max);
        }

        template<class... Args>
        static void DragFloat(const std::string& text,
                              float&             value,
                              const float&       speed,
                              const float&       v_min,
                              const float&       v_max,
                              Args... args)
        {
            ImGui::DragFloat(fmt::format(text, args...).c_str(), &value, speed, v_min, v_max);
        }

        template<class... Args>
        static void DragFloat3(const std::string& text,
                               glm::vec3&         values,
                               const float&       speed,
                               const float&       v_min,
                               const float&       v_max,
                               Args... args)
        {
            ImGui::DragFloat3(fmt::format(text, args...).c_str(), glm::value_ptr(values), speed, v_min, v_max);
        }

        static void ShowImNodesDemoWindow();

        static void DockSpace(bool& app_open_ref);

        template<class... Args>
        static void ShowViewport(const std::string&     text,
                                 Ref<FrameRenderBuffer> framebuffer,
                                 const bool             show_overlap,
                                 bool&                  is_focused,
                                 Args... args)
        {
            ImGui::Begin(fmt::format(text, args...).c_str());
            {
                ImGui::BeginChild("Render");
                is_focused   = ImGui::IsWindowFocused();
                ImVec2 wsize = ImGui::GetWindowSize();
                framebuffer->SetViewPort((uint32_t)wsize.x, (uint32_t)wsize.y);
                ImGui::Image(framebuffer->GetTextureID(), wsize, ImVec2(0, 1), ImVec2(1, 0));

                if (show_overlap)
                {}

                ImGui::EndChild();
            }
            ImGui::End();
        }
    };
    using Gui = GuiCommand;
} // namespace Engine
