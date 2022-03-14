#pragma once

#include "Application.h"
#include "Core.h"
#include "Input.h"
#include "Layer.h"
#include "Log.h"
#include "Renderer.h"

#include "Gui/GuiCommand.h"
#include "Gui/DockSpaceLayer.h"
#include "Gui/ImGuiLog.h"

#include "Utils/Spare.h"
#include "Utils/Octree.h"

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