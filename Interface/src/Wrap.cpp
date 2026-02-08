#include "Wrap.h"
#include <glm/common.hpp>
#include <kotono_common/bitwise_utils.h>

WWrap::WWrap(const WrapSettings& wrapSettings) :
	WChildOwnerWidget(wrapSettings.child),
	wrapSettings_(wrapSettings)
{
}

UWidgetDisplaySettings WWrap::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (wrapSettings_.child)
	{
		auto childDesiredSize{ wrapSettings_.child->GetDesiredSize(displaySettings.bounds) };
		if (has_flag(wrapSettings_.axis, EAxis::Horizontal))
		{
			displaySettings.bounds.x = std::min(displaySettings.bounds.x, childDesiredSize.x);
		}
		if (has_flag(wrapSettings_.axis, EAxis::Vertical))
		{
			displaySettings.bounds.y = std::min(displaySettings.bounds.y, childDesiredSize.y);
		}
		return wrapSettings_.child->GetDisplaySettings(displaySettings);
	}

	return displaySettings;
}

EFlex WWrap::GetFlex() const
{
	switch (wrapSettings_.axis)
	{
	case EAxis::Horizontal:	return EFlex::Vertical;
	case EAxis::Vertical:	return EFlex::Horizontal;
	case EAxis::All:		return EFlex::None;
	default:				return EFlex::None;
	}
}

void WWrap::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (wrapSettings_.child)
	{
		wrapSettings_.child->Display(displaySettings);
	}
}
