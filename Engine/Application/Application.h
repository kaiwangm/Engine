#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "ImGuiLayer.h"
#include "LayerStack.h"
#include "Window.h"

namespace Engine {
class Application {
   public:
    Application(std::string appName, uint32_t windowWidth,
                uint32_t windowHeight);
    virtual ~Application();

    void Run();

    void OnEvent(Event& event);

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> layer);

    inline Window& GetWindow() { return *m_Window; }

    static Application& Get() { return *s_Instance; }

    // Event Callbacks
   private:
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResizeEvent(WindowResizeEvent& event);

   private:
    Scope<Window> m_Window;
    LayerStack m_LayerStack;
    bool m_Running = true;
    Ref<ImGuiLayer> m_ImGuiLayer;
    static Application* s_Instance;

   private:
    bool m_Minimized;

   protected:
    Timer m_Timer;
};

Scope<Application> CreateApplication();
}  // namespace Engine