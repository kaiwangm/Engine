#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "ImGuiLayer.h"
#include "LayerStack.h"
#include "Window.h"

namespace Engine {
class Application {
   public:
    Application();
    virtual ~Application();

    void Run();

    void onEvent(Event& e);

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> layer);

    inline Window& GetWindow() { return *m_Window; }

    static Application& Get() { return *s_Instance; }

    // Event Callbacks
   private:
    bool onWindowClose(WindowCloseEvent& e);

   private:
    Scope<Window> m_Window;
    LayerStack m_LayerStack;
    bool m_Running = true;
    Ref<ImGuiLayer> m_ImGuiLayer;
    static Application* s_Instance;

   protected:
    Timer m_Timer;
};

Scope<Application> CreateApplication();
}  // namespace Engine