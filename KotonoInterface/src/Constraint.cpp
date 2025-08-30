#include "Constraint.h"

WConstraint::WConstraint(const ConstraintSettings& constraintSettings) :
	WChildOwnerWidget(constraintSettings.child),
	constraintSettings_(constraintSettings)
{
}

void WConstraint::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);
	displaySettings = GetDisplaySettings(displaySettings);

	++displaySettings.layer;

	if (constraintSettings_.child)
	{
		constraintSettings_.child->Display(displaySettings);
	}
}

WWidget::DisplaySettings WConstraint::GetDisplaySettings(DisplaySettings displaySettings) const
{
	switch (constraintSettings_.axis)
	{
	case Axis::Horizontal:
		displaySettings.bounds.x = std::min(constraintSettings_.size, displaySettings.bounds.x);
		break;
	case Axis::Vertical:
		displaySettings.bounds.y = std::min(constraintSettings_.size, displaySettings.bounds.y);
		break;
	}

	if (constraintSettings_.child)
	{
		return constraintSettings_.child->GetDisplaySettings(displaySettings);
	}
	return displaySettings;
}
