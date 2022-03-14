#pragma once
#include "Application.h"

#ifdef ENGINE_PLATFORM_WINDOWS

#ifdef ENGINE_USE_ENTRYPOINT
namespace Engine {

extern Scope<Application> CreateApplication();
}

int main(int argc, char **argv) {
    auto app = Engine::CreateApplication();
    app->Run();

    return 0;
}
#endif

#endif