#include "Center.h"

WCenter::WCenter(const CenterSettings& centerSettings) :
	WChildOwnerWidget(centerSettings.child),
	centerSettings_(centerSettings)
{
}

void WCenter::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (centerSettings_.child)
	{
		centerSettings_.child->Display(displaySettings);
	}
}

UWidgetDisplaySettings WCenter::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{

	if (centerSettings_.child)
	{
		const auto childSettings{ centerSettings_.child->GetDisplaySettings(displaySettings) };

		switch (centerSettings_.axis)
		{
		case EAxis::Horizontal:
			displaySettings.position.x = displaySettings.position.x + (displaySettings.bounds.x - childSettings.bounds.x) / 2.0f;
			break;
		case EAxis::Vertical:
			displaySettings.position.y = displaySettings.position.y + (displaySettings.bounds.y - childSettings.bounds.y) / 2.0f;
			break;
		case EAxis::All:
			displaySettings.position = displaySettings.position + (displaySettings.bounds - childSettings.bounds) / 2.0f;
			break;
		}
	}

	return displaySettings;
}
