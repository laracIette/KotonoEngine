#include "Padding.h"
#include <kotono_common/log.h>

WPadding::WPadding(const PaddingSettings& paddingSettings) :
	WChildOwnerWidget(paddingSettings.child),
	paddingSettings_(paddingSettings)
{
}

void WPadding::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (paddingSettings_.child)
	{
		paddingSettings_.child->Display(displaySettings);
	}
}

UWidgetDisplaySettings WPadding::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds.x -= paddingSettings_.padding.l;
	displaySettings.bounds.x -= paddingSettings_.padding.r;
	displaySettings.bounds.y -= paddingSettings_.padding.t;
	displaySettings.bounds.y -= paddingSettings_.padding.b;

	displaySettings.position.x += paddingSettings_.padding.l;
	displaySettings.position.y += paddingSettings_.padding.t;
	
	return displaySettings;
}

glm::vec2 WPadding::GetDesiredSize(glm::vec2 bounds) const
{
	if (paddingSettings_.child)
	{
		auto childDesiredSize{ paddingSettings_.child->GetDesiredSize(bounds) };
		childDesiredSize.x += paddingSettings_.padding.l + paddingSettings_.padding.r;
		childDesiredSize.y += paddingSettings_.padding.t + paddingSettings_.padding.b;
		return childDesiredSize;
	}

	return { 0.0f, 0.0f };
}
