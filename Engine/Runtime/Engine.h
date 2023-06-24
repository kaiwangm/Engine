#pragma once

#include "Application.h"
#include "Core.h"
#include "Input/Input.h"
#include "Layer/Layer.h"
#include "Renderer.h"

#include "Gui/GuiCommand.h"
#include "Gui/DockSpaceLayer.h"
#include "Gui/ImGuiLog.h"

#include "Utils/Spare.h"
#include "Utils/Octree.h"

#include "GameFramework/Animation/AAnimatedMesh.h"
#include "GameFramework/StaticMesh/AStaticMesh.h"
#include "GameFramework/StaticMesh/APointCloud.h"

#include "GameFramework/Camera/ACamera.h"

#include "GameFramework/Skybox/ASkybox.h"
#include "GameFramework/Light/APointLight.h"
#include "GameFramework/Material/MBasicPbr.h"

#ifdef ENGINE_PLATFORM_WINDOWS

#ifdef ENGINE_USE_ENTRYPOINT
namespace Engine {

extern Scope<Application> CreateApplication(int argc, char** argv);
}

int main(int argc, char **argv) {
    auto app = Engine::CreateApplication(argc, argv);
    app->Run();

    return 0;
}
#endif

#endif