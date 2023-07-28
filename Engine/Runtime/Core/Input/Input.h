#pragma once

#include <Engine/Runtime/Core/Core.h>

namespace Engine
{
    class Input
    {
    public:
        static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

        static bool IsMouseButtonPressed(int keycode) { return s_Instance->IsMouseButtonPressedImpl(keycode); }

        static bool IsMouseButtonReleased(int keycode) { return !s_Instance->IsMouseButtonPressedImpl(keycode); }

        static std::pair<float, float> GetMousePostion() { return s_Instance->GetMousePostionImpl(); }

        static float GetMouseX() { return s_Instance->GetMouseXImpl(); }

        static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

        static void HideCursor() { s_Instance->HideCursorImpl(); }

    protected:
        virtual bool                    IsKeyPressedImpl(int keycode)         = 0;
        virtual bool                    IsMouseButtonPressedImpl(int keycode) = 0;
        virtual std::pair<float, float> GetMousePostionImpl()                 = 0;
        virtual float                   GetMouseXImpl()                       = 0;
        virtual float                   GetMouseYImpl()                       = 0;
        virtual void                    HideCursorImpl()                      = 0;

    private:
        static Input* s_Instance;
    };
} // namespace Engine