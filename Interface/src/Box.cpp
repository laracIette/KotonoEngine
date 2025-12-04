#include "Box.h"
#include <glm/common.hpp>

WBox::WBox(const BoxSettings& boxSettings) :
	WChildOwnerWidget(boxSettings.child),
	boxSettings_(boxSettings)
{
}

void WBox::DisplayInternal(DisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (boxSettings_.child)
	{
		boxSettings_.child->Display(displaySettings);
	}
}

WWidget::DisplaySettings WBox::GetDisplaySettings(DisplaySettings displaySettings) const
{
	displaySettings.bounds = glm::min(boxSettings_.size, displaySettings.bounds);
	return displaySettings;
}
