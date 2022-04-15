#pragma once
#include "Camera.h"

namespace Engine {
class UCameraComponent : public UComponent {
   public:
    Ref<Camera> m_Camera;

    UCameraComponent(const Ref<Camera> camera) : m_Camera(camera) {}

    Ref<Camera> GetCamera() { return m_Camera; }
};
};  // namespace Engine
