#pragma once

#include <Engine/Runtime/Application/Application.h>
#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Core/Input/Input.h>
#include <Engine/Runtime/Core/Layer/Layer.h>
#include <Engine/Runtime/Renderer/Renderer.h>

#include <Engine/Runtime/Gui/GuiCommand.h>
#include <Engine/Runtime/Gui/DockSpaceLayer.h>
#include <Engine/Runtime/Gui/ImGuiLog.h>

#include <Engine/Runtime/Utils/Spare.h>
#include <Engine/Runtime/Utils/Octree.h>

#include <Engine/Runtime/GameFramework/Animation/AAnimatedMesh.h>
#include <Engine/Runtime/GameFramework/Animation/ASkeleton.h>

#include <Engine/Runtime/GameFramework/StaticMesh/AStaticMesh.h>
#include <Engine/Runtime/GameFramework/StaticMesh/APointCloud.h>

#include <Engine/Runtime/GameFramework/Camera/ACamera.h>
#include <Engine/Runtime/GameFramework/Pawn/APawn.h>

#include <Engine/Runtime/GameFramework/Skybox/ASkybox.h>
#include <Engine/Runtime/GameFramework/Light/APointLight.h>
#include <Engine/Runtime/GameFramework/Material/MBasicPbr.h>

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





















