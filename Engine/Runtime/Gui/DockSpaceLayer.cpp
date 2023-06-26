#include <Engine/Runtime/Gui/DockSpaceLayer.h>
#include <Engine/Runtime/Gui/GuiCommand.h>

namespace Engine
{
    void DockSpaceLayer::TickGui() { Gui::DockSpace(m_app_open); }
} // namespace Engine