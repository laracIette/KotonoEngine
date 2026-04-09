#include "Expanded.h"

void WExpanded::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (child_)
	{
		child_->Display(displaySettings);
	}
}

UWidgetDisplaySettings WExpanded::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	return displaySettings;
}

EFlex WExpanded::GetFlex() const
{
	return EFlex::All;
}

#include "generated/Expanded.generated.inl"
