#include "Offset.h"

WOffset::WOffset(const OffsetSettings& offsetSettings) :
	WChildOwnerWidget(offsetSettings.child),
	offsetSettings_(offsetSettings)
{
}

void WOffset::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (offsetSettings_.child)
	{
		offsetSettings_.child->Display(displaySettings);
	}
}

UWidgetDisplaySettings WOffset::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.position += offsetSettings_.offset;
	
	if (offsetSettings_.child)
	{
		return offsetSettings_.child->GetDisplaySettings(displaySettings);
	}
	return displaySettings;
}
