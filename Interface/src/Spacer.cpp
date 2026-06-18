#include "Spacer.h"
#include <kotono_common/enum_utils.h>

WSpacer::WSpacer(const EAxis axis)
	: axis_(axis)
{
}

UWidgetDisplaySettings WSpacer::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (has_flag(axis_, EAxis::Horizontal))
	{
		displaySettings.bounds.y = 0.0f;
	}
	if (has_flag(axis_, EAxis::Vertical))
	{
		displaySettings.bounds.x = 0.0f;
	}
	
	return displaySettings;
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
