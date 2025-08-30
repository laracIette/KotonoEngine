#include "Expanded.h"

WExpanded::WExpanded(const ExpandedSettings& expandedSettings) :
	WChildOwnerWidget(expandedSettings.child),
	expandedSettings_(expandedSettings)
{
}

void WExpanded::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);
	displaySettings = GetDisplaySettings(displaySettings);

	if (expandedSettings_.child)
	{
		expandedSettings_.child->Display(displaySettings);
	}
}

WWidget::DisplaySettings WExpanded::GetDisplaySettings(DisplaySettings displaySettings) const
{
	return displaySettings;
}
