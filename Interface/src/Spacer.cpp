#include "Spacer.h"
#include <kotono_common/enum_utils.h>

WSpacer::WSpacer(EAxis axis)
	: axis_{ axis }
{
}

glm::vec2 WSpacer::GetContentSize(glm::vec2 bounds) const
{
	if (has_flag(axis_, EAxis::Horizontal))
	{
		bounds.y = 0.0f;
	}
	if (has_flag(axis_, EAxis::Vertical))
	{
		bounds.x = 0.0f;
	}
	
	return bounds;
}

EExpand WSpacer::GetExpand() const
{
	switch (axis_)
	{
	case EAxis::All:		return EExpand::All;
	case EAxis::Horizontal: return EExpand::Horizontal;
	case EAxis::Vertical:	return EExpand::Vertical;
	default:				return EExpand::None;
	}
}

EFlex WSpacer::GetFlex() const
{
	switch (axis_)
	{
	case EAxis::All:		return EFlex::All;
	case EAxis::Horizontal: return EFlex::Horizontal;
	case EAxis::Vertical:	return EFlex::Vertical;
	default:				return EFlex::None;
	}
}

#include "generated/Spacer.generated.inl"
