#include "Box.h"

WBox::WBox(const BoxSettings& boxSettings) :
	WChildOwnerWidget(boxSettings.child),
	boxSettings_(boxSettings)
{
}

void WBox::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	displaySettings.bounds = boxSettings_.size;
	++displaySettings.layer;

	if (boxSettings_.child)
	{
		boxSettings_.child->Display(displaySettings);
	}
}

WWidget::DisplaySettings WBox::GetDisplaySettings(DisplaySettings displaySettings)
{
	displaySettings.bounds = glm::min(boxSettings_.size, displaySettings.bounds);
	
	if (boxSettings_.child)
	{
		return boxSettings_.child->GetDisplaySettings(displaySettings);
	}
	return displaySettings;
}
