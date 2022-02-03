#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

#ifdef ENGINE_USE_ENTRYPOINT
int main(int argc, char **argv)
{
    auto app = Engine::CreateApplication();
    app->Run();
    delete app;
}
#endif

#endif