#include "Spacer.h"
#include <kotono_common/bitwise_utils.h>

WSpacer::WSpacer(const EFlex flex)
	: flex_(flex)
{
}

UWidgetDisplaySettings WSpacer::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (has_flag(flex_, EFlex::Horizontal))
	{
		displaySettings.bounds.y = 0.0f;
	}
	if (has_flag(flex_, EFlex::Vertical))
	{
		displaySettings.bounds.x = 0.0f;
	}
	
	return displaySettings;
}

EFlex WSpacer::GetFlex() const
{
	return flex_;
}

#include "generated/Spacer.generated.inl"
