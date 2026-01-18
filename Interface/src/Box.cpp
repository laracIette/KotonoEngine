#include "Box.h"
#include <glm/common.hpp>

WBox::WBox(const BoxSettings& boxSettings) :
	WChildOwnerWidget(boxSettings.child),
	boxSettings_(boxSettings)
{
}

void WBox::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (boxSettings_.child)
	{
		boxSettings_.child->Display(displaySettings);
	}
}

UWidgetDisplaySettings WBox::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds = glm::min(boxSettings_.size, displaySettings.bounds);
	return displaySettings;
}

EFlex WBox::GetFlex() const
{
	return EFlex::None;
}

glm::vec2 WBox::GetDesiredSize() const
{
	return boxSettings_.size;
}
