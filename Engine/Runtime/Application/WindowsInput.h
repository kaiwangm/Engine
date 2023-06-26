#pragma once
#include <Engine/Runtime/Core/Input/Input.h>

namespace Engine
{
    class WindowsInput : public Input
    {
    protected:
        virtual bool                    IsKeyPressedImpl(int keycode) override;
        virtual bool                    IsMouseButtonPressedImpl(int keycode) override;
        virtual std::pair<float, float> GetMousePostionImpl() override;
        virtual float                   GetMouseXImpl() override;
        virtual float                   GetMouseYImpl() override;
    };
} // namespace Engine