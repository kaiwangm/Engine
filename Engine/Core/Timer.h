#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
#include <GLFW/glfw3.h>
#endif

namespace Engine {
class Timer {
   public:
    Timer(float time = 0.0f) : m_Time(time), m_LastFrameTime(time) {}
    float GetSeconds() const { return m_Time; }
    float GetMilliseconds() const { return m_Time * 1000.0f; }
    float GetTimeStep() {
        m_Time = (float)glfwGetTime();
        float timeStep = m_Time - m_LastFrameTime;
        m_LastFrameTime = m_Time;

        return timeStep;
    }

   private:
    float m_Time;
    float m_LastFrameTime;
};
}  // namespace Engine