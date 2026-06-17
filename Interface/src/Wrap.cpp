#include "Wrap.h"
#include <glm/common.hpp>
#include <kotono_common/bitwise_utils.h>

WWrap::WWrap(const EAxis axis)
	: axis_(axis)
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

EExpand WWrap::GetExpand() const
{
	switch (axis_)
	{
	case EAxis::Horizontal:	return EExpand::Vertical;
	case EAxis::Vertical:	return EExpand::Horizontal;
	default:				return EExpand::None;
	}
}

EFlex WWrap::GetFlex() const
{
	return EFlex::All;
}

#include "generated/Wrap.generated.inl"
