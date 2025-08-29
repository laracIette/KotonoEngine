#include "Offset.h"

WOffset::WOffset(const OffsetSettings& offsetSettings) :
	WChildOwnerWidget(offsetSettings.child),
	offsetSettings_(offsetSettings)
{
}

void WOffset::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	displaySettings.position += offsetSettings_.size;
	++displaySettings.layer;

	if (offsetSettings_.child)
	{
		offsetSettings_.child->Display(displaySettings);
	}
}

WWidget::DisplaySettings WOffset::GetDisplaySettings(DisplaySettings displaySettings)
{
	displaySettings.position += offsetSettings_.size;
	
	if (offsetSettings_.child)
	{
		return offsetSettings_.child->GetDisplaySettings(displaySettings);
	}
	return displaySettings;
}
