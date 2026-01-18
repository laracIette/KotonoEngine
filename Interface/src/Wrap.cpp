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
		displaySettings.bounds = glm::min(displaySettings.bounds, childDesiredSize);
		return wrapSettings_.child->GetDisplaySettings(displaySettings);
	}

	return displaySettings;
}

EFlex WWrap::GetFlex() const
{
	return EFlex::None;
}

void WWrap::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (wrapSettings_.child)
	{
		wrapSettings_.child->Display(displaySettings);
	}
}
