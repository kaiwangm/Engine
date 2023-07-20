#pragma once

#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Gui/ImNodes/ImGuiNodes.h>
#include <Engine/Runtime/Renderer/Renderer.h>

namespace Engine
{
    class GuiCommand
    {
    public:
        static void Begin(const std::string& name) { ImGui::Begin(name.c_str()); }

        static void End() { ImGui::End(); }

        static void Text(const std::string& text) { ImGui::TextUnformatted(text.c_str()); }

        static void ColorEdit4(const std::string& text, glm::vec4& color)
        {
            ImGui::ColorEdit4(text.c_str(), glm::value_ptr(color));
        }

        static void SliderFloat(const std::string& text, float& value, const float& v_min, const float& v_max)
        {
            ImGui::SliderFloat(text.c_str(), &value, v_min, v_max);
        }

        static void SliderFloat3(const std::string& text, glm::vec3& values, const float& v_min, const float& v_max)
        {
            ImGui::SliderFloat3(text.c_str(), glm::value_ptr(values), v_min, v_max);
        }

        static void
        DragFloat(const std::string& text, float& value, const float& speed, const float& v_min, const float& v_max)
        {
            ImGui::DragFloat(text.c_str(), &value, speed, v_min, v_max);
        }

        static void DragFloat3(const std::string& text,
                               glm::vec3&         values,
                               const float&       speed,
                               const float&       v_min,
                               const float&       v_max)
        {
            ImGui::DragFloat3(text.c_str(), glm::value_ptr(values), speed, v_min, v_max);
        }

        static void ShowImNodesDemoWindow();

        static void DockSpace(bool& app_open_ref, std::unordered_map<std::string, std::function<void()>>& callbacks);

        static void ShowViewport(const std::string&     text,
                                 Ref<FrameRenderBuffer> framebuffer,
                                 const bool             show_overlap,
                                 bool&                  is_focused)
        {
            ImGui::Begin(text.c_str());
            {
                ImGui::BeginChild("Render");
                is_focused   = ImGui::IsWindowFocused();
                ImVec2 wsize = ImGui::GetWindowSize();
                framebuffer->SetViewPort((uint32_t)wsize.x, (uint32_t)wsize.y);
                ImGui::Image(framebuffer->GetTextureID(), wsize, ImVec2(0, 1), ImVec2(1, 0));

                ImGui::EndChild();
            }
            ImGui::End();
        }

        static void ShowViewport(const std::string&     text,
                                 Ref<FrameRenderBuffer> framebuffer,
                                 const bool             show_overlap,
                                 bool&                  is_focused,
                                 std::function<void()>  callback)
        {
            ImGui::Begin(text.c_str());
            {
                ImGui::BeginChild("Render");
                is_focused   = ImGui::IsWindowFocused();
                ImVec2 wsize = ImGui::GetWindowSize();
                framebuffer->SetViewPort((uint32_t)wsize.x, (uint32_t)wsize.y);
                ImGui::Image(framebuffer->GetTextureID(), wsize, ImVec2(0, 1), ImVec2(1, 0));
                callback();
                ImGui::EndChild();
            }

            ImGui::End();
        }
    };
    using Gui = GuiCommand;
} // namespace Engine
