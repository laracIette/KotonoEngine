#include "Center.h"

WCenter::WCenter(const EAxis axis)
	: axis_{ axis }
{
}

UWidgetDisplaySettings WCenter::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (child_)
	{
		const auto childSettings{ child_->GetContentDisplaySettings(displaySettings) };

		switch (axis_)
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

#include "generated/Center.generated.inl"
