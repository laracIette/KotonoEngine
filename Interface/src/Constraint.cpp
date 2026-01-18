#include "Constraint.h"

WConstraint::WConstraint(const ConstraintSettings& constraintSettings) :
	WChildOwnerWidget(constraintSettings.child),
	constraintSettings_(constraintSettings)
{
}

void WConstraint::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (constraintSettings_.child)
	{
		constraintSettings_.child->Display(displaySettings);
	}
}

UWidgetDisplaySettings WConstraint::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
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

EFlex WConstraint::GetFlex() const
{
	switch (constraintSettings_.axis)
	{
	case Axis::Horizontal:	return EFlex::Vertical;
	case Axis::Vertical:	return EFlex::Horizontal;
	default:				return EFlex::None;
	}
}

glm::vec2 WConstraint::GetDesiredSize() const
{
	switch (constraintSettings_.axis)
	{
	case Axis::Horizontal:	return { constraintSettings_.size, 0.0f };
	case Axis::Vertical:	return { 0.0f, constraintSettings_.size };
	default:				return { 0.0f, 0.0f };
	}
}
