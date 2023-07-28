#include <Engine/Runtime/Application/WindowsInput.h>
#include <Engine/Runtime/Application/Application.h>
#include <imgui.h>

namespace Engine
{
    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::IsKeyPressedImpl(int keycode)
    {
        auto glfw_window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state       = glfwGetKey(glfw_window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int keycode)
    {
        auto glfw_window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state       = glfwGetMouseButton(glfw_window, keycode);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::GetMousePostionImpl()
    {
        auto   glfw_window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double x, y;
        glfwGetCursorPos(glfw_window, &x, &y);

        return {(float)x, (float)y};
    }

    float WindowsInput::GetMouseXImpl()
    {
        auto [x, y] = GetMousePostionImpl();

        return x;
    }

    float WindowsInput::GetMouseYImpl()
    {
        auto [x, y] = GetMousePostionImpl();

        return y;
    }

    void WindowsInput::HideCursorImpl() { ImGui::SetMouseCursor(ImGuiMouseCursor_None); }
} // namespace Engine