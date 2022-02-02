#include <Engine.h>

class Sandbox: public Engine::Application
{
public:
    Sandbox()
    {
        ENGINE_CORE_TRACE("Sandbox Initialization");
    }

    ~Sandbox()
    {
        ENGINE_CORE_TRACE("Sandbox Shutdown");
    }

    void Run()
    {
        for(int i = 0; i < 10; ++i)
        {
            ENGINE_CORE_INFO("Run");
        }
    }
};

Engine::Application* Engine::CreateApplication()
{
    return new Sandbox();
}
