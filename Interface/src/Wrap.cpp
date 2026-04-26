#include "Wrap.h"
#include <glm/common.hpp>
#include <kotono_common/bitwise_utils.h>

WWrap::WWrap()
	: axis_(EAxis::All)
{
}

UWidgetDisplaySettings WWrap::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (child_)
	{
		auto childDesiredSize{ child_->GetDesiredSize(displaySettings.bounds) };
		if (has_flag(axis_, EAxis::Horizontal))
		{
			displaySettings.bounds.x = std::min(displaySettings.bounds.x, childDesiredSize.x);
		}
		if (has_flag(axis_, EAxis::Vertical))
		{
			displaySettings.bounds.y = std::min(displaySettings.bounds.y, childDesiredSize.y);
		}
		return child_->GetContentDisplaySettings(displaySettings);
	}

	return displaySettings;
}

EFlex WWrap::GetFlex() const
{
	switch (axis_)
	{
	case EAxis::Horizontal:	return EFlex::Vertical;
	case EAxis::Vertical:	return EFlex::Horizontal;
	case EAxis::All:		return EFlex::None;
	default:				return EFlex::None;
	}
}

#include "generated/Wrap.generated.inl"
