#include "Constraint.h"

WConstraint::WConstraint(EAxis axis, f32 size)
	: axis_{ axis }
	, size_{ size }
{
}

WConstraint::WConstraint()
	: Self(EAxis::All, 64.0f)
{
}

glm::vec2 WConstraint::GetContentSize(glm::vec2 bounds) const
{
	switch (axis_)
	{
	case EAxis::Horizontal:
		bounds.x = std::min(size_, bounds.x);
		break;
	case EAxis::Vertical:
		bounds.y = std::min(size_, bounds.y);
		break;
	}

	return Base::GetContentSize(bounds);
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

void WConstraint::DisplayInternal(UWidgetDisplaySettings displaySettings)
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

	Base::DisplayInternal(displaySettings);
}

#include "generated/Constraint.generated.inl"
