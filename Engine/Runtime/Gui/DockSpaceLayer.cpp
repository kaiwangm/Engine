#include "DockSpaceLayer.h"

#include "GuiCommand.h"

namespace Engine {
void DockSpaceLayer::TickGui() { Gui::DockSpace(m_app_open); }
}  // namespace Engine