#include "Expanded.h"

UWidgetDisplaySettings WExpanded::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	return displaySettings;
}

EFlex WExpanded::GetFlex() const
{
	return EFlex::All;
}

#include "generated/Expanded.generated.inl"
