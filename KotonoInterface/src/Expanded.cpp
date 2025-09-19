#include "Expanded.h"

WExpanded::WExpanded(const ExpandedSettings& expandedSettings) :
	WChildOwnerWidget(expandedSettings.child),
	expandedSettings_(expandedSettings)
{
}

void WExpanded::DisplayInternal(DisplaySettings displaySettings)
{
	if (expandedSettings_.child)
	{
		expandedSettings_.child->Display(displaySettings);
	}
}

WWidget::DisplaySettings WExpanded::GetDisplaySettings(DisplaySettings displaySettings) const
{
	return displaySettings;
}

EFlex WExpanded::GetFlex() const
{
	return EFlex::All;
}
