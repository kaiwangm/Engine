#include "DockSpaceLayer.h"

#include "GuiCommand.h"

namespace Engine {
void DockSpaceLayer::TickRender() { Gui::DockSpace(m_app_open); }
}  // namespace Engine