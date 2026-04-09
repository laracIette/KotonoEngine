#include "Center.h"

UWidgetDisplaySettings WCenter::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{

	if (child_)
	{
		const auto childSettings{ child_->GetDisplaySettings(displaySettings) };

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

EAxis WCenter::GetAxis() const
{
	return axis_;
}

void WCenter::SetAxis(const EAxis axis)
{
	axis_ = axis;
}

void WCenter::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (child_)
	{
		child_->Display(displaySettings);
	}
}

#include "generated/Center.generated.inl"
