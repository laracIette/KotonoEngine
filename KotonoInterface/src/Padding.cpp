#include "Padding.h"

WPadding::WPadding(const PaddingSettings& paddingSettings) :
	WChildOwnerWidget(paddingSettings.child),
	paddingSettings_(paddingSettings)
{
}

void WPadding::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);
	displaySettings = GetDisplaySettings(displaySettings);

	++displaySettings.layer;

	if (paddingSettings_.child)
	{
		paddingSettings_.child->Display(displaySettings);
	}
}

WWidget::DisplaySettings WPadding::GetDisplaySettings(DisplaySettings displaySettings) const
{
	displaySettings.bounds.x -= paddingSettings_.padding.l;
	displaySettings.bounds.x -= paddingSettings_.padding.r;
	displaySettings.bounds.y -= paddingSettings_.padding.t;
	displaySettings.bounds.y -= paddingSettings_.padding.b;

	displaySettings.position.x += paddingSettings_.padding.l;
	displaySettings.position.y += paddingSettings_.padding.t;
	
	//if (paddingSettings_.child)
	//{
	//	return paddingSettings_.child->GetDisplaySettings(displaySettings);
	//}
	return displaySettings;
}
