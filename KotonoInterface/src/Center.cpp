#include "Center.h"

WCenter::WCenter(const CenterSettings& centerSettings) :
	WChildOwnerWidget(centerSettings.child),
	centerSettings_(centerSettings)
{
}

void WCenter::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	switch (centerSettings_.axis)
	{
	case WCenter::Axis::All:
		displaySettings.position = (displaySettings.position + displaySettings.bounds) / 2.0f;
		break;
	case WCenter::Axis::Horizontal:
		displaySettings.position.x = (displaySettings.position.x + displaySettings.bounds.x) / 2.0f;
		break;
	case WCenter::Axis::Vertical:
		displaySettings.position.y = (displaySettings.position.y + displaySettings.bounds.y) / 2.0f;
		break;
	}
	++displaySettings.layer;

	if (centerSettings_.child)
	{
		centerSettings_.child->Display(displaySettings);
	}
}

WWidget::DisplaySettings WCenter::GetDisplaySettings(DisplaySettings displaySettings)
{
	switch (centerSettings_.axis)
	{
	case WCenter::Axis::All:
		displaySettings.position = (displaySettings.position + displaySettings.bounds) / 2.0f;
		break;
	case WCenter::Axis::Horizontal:
		displaySettings.position.x = (displaySettings.position.x + displaySettings.bounds.x) / 2.0f;
		break;
	case WCenter::Axis::Vertical:
		displaySettings.position.y = (displaySettings.position.y + displaySettings.bounds.y) / 2.0f;
		break;
	}

	if (centerSettings_.child)
	{
		return centerSettings_.child->GetDisplaySettings(displaySettings);
	}
	return displaySettings;
}
