#include "Constraint.h"

UWidgetDisplaySettings WConstraint::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	switch (axis_)
	{
	case EAxis::Horizontal:
		displaySettings.bounds.x = std::min(size_, displaySettings.bounds.x);
		break;
	case EAxis::Vertical:
		displaySettings.bounds.y = std::min(size_, displaySettings.bounds.y);
		break;
	}

	if (child_)
	{
		return child_->GetContentDisplaySettings(displaySettings);
	}
	return displaySettings;
}

glm::vec2 WConstraint::GetDesiredSize(const glm::vec2& bounds) const
{
	switch (axis_)
	{
	case EAxis::Horizontal:	return { size_, 0.0f };
	case EAxis::Vertical:	return { 0.0f, size_ };
	default:				return { 0.0f, 0.0f };
	}
}

EExpand WConstraint::GetExpand() const
{
	switch (axis_)
	{
	case EAxis::Horizontal:	return EExpand::Vertical;
	case EAxis::Vertical:	return EExpand::Horizontal;
	default:				return EExpand::None;
	}
}

EFlex WConstraint::GetFlex() const
{
	switch (axis_)
	{
	case EAxis::Horizontal:	return EFlex::Vertical;
	case EAxis::Vertical:	return EFlex::Horizontal;
	default:				return EFlex::None;
	}
}

#include "generated/Constraint.generated.inl"
