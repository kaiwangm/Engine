#include "DockSpaceLayer.h"
#include "GuiCommand.h"

namespace Engine
{
    void DockSpaceLayer::OnImGuiRender()
    {
        Gui::DockSpace(m_app_open);
    }
}