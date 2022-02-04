#include <Engine.h>

class Sandbox: public Engine::Application
{
public:
    Sandbox()
    {
        ENGINE_TRACE("Sandbox Initialization");
    }

    ~Sandbox()
    {
        ENGINE_TRACE("Sandbox Shutdown");
    }
};

Engine::Application* Engine::CreateApplication()
{
    return new Sandbox();
}
