#include "Spacer.h"
#include "Row.h"
#include "Column.h"

WSpacer::WSpacer(const SpacerSettings& spacerSettings) :
	spacerSettings_(spacerSettings)
{
}

UWidgetDisplaySettings WSpacer::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (dynamic_cast<const WRow*>(parent_))
	{
		displaySettings.bounds.y = 0.0f;
	}
	else if (dynamic_cast<const WColumn*>(parent_))
	{
		displaySettings.bounds.x = 0.0f;
	}
	return displaySettings;
}

EFlex WSpacer::GetFlex() const
{
	if (dynamic_cast<const WRow*>(parent_))
	{
		return EFlex::Horizontal;
	}
	if (dynamic_cast<const WColumn*>(parent_))
	{
		return EFlex::Vertical;
	}
	return EFlex::None;
}
