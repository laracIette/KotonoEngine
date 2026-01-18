#include "Wrap.h"

WWrap::WWrap(const WrapSettings& wrapSettings) :
	WChildOwnerWidget(wrapSettings.child),
	wrapSettings_(wrapSettings)
{
}

UWidgetDisplaySettings WWrap::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (wrapSettings_.child)
	{
		displaySettings.bounds = wrapSettings_.child->GetDesiredSize();
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
