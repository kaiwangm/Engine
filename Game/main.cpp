#include <Engine.h>

#include <glm/vec3.hpp>

class Sandbox: public Engine::Application
{
public:
    Sandbox()
    {

        auto vec = glm::vec3(-1.0f, 1.0f, 0.9f);
        ENGINE_TRACE(vec.x);

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
