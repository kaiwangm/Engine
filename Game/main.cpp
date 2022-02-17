#include <Engine.h>

class Sandbox: public Engine::Application
{
public:
    Sandbox()
    {
        ENGINE_TRACE("Sandbox Initialization.");
    }

    ~Sandbox()
    {
        ENGINE_TRACE("Sandbox Shutdown.");
    }

    void ClientDraw() override
    {
        //glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
    }
};

Engine::Application* Engine::CreateApplication()
{
    return new Sandbox();
}
