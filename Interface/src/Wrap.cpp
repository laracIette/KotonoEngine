#include "Wrap.h"
#include <glm/common.hpp>

WWrap::WWrap(const WrapSettings& wrapSettings) :
	WChildOwnerWidget(wrapSettings.child),
	wrapSettings_(wrapSettings)
{
}

UWidgetDisplaySettings WWrap::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (wrapSettings_.child)
	{
		auto childDesiredSize{ wrapSettings_.child->GetDesiredSize() };
		switch (wrapSettings_.axis)
		{
		case Axis::Horizontal:
			displaySettings.bounds.x = std::min(displaySettings.bounds.x, childDesiredSize.x);
			break;
		case Axis::Vertical:
			displaySettings.bounds.y = std::min(displaySettings.bounds.y, childDesiredSize.y);
			break;
		case Axis::All:
			displaySettings.bounds = glm::min(displaySettings.bounds, childDesiredSize);
			break;
		}
		return wrapSettings_.child->GetDisplaySettings(displaySettings);
	}

	return displaySettings;
}

EFlex WWrap::GetFlex() const
{
	switch (wrapSettings_.axis)
	{
	case Axis::Horizontal:	return EFlex::Vertical;
	case Axis::Vertical:	return EFlex::Horizontal;
	case Axis::All:			return EFlex::None;
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
