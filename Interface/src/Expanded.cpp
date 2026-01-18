#include "Expanded.h"

WExpanded::WExpanded(const ExpandedSettings& expandedSettings) :
	WChildOwnerWidget(expandedSettings.child),
	expandedSettings_(expandedSettings)
{
}

void WExpanded::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (expandedSettings_.child)
	{
		expandedSettings_.child->Display(displaySettings);
	}
}

UWidgetDisplaySettings WExpanded::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	return displaySettings;
}

EFlex WExpanded::GetFlex() const
{
	return EFlex::All;
}
