#include "Spacer.h"
#include <kotono_common/bitwise_utils.h>

UWidgetDisplaySettings WSpacer::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (parent_)
	{
		if (has_flag(parent_->GetFlex(), EFlex::Horizontal)) // todo: check if works
		{
			displaySettings.bounds.y = 0.0f;
		}
		if (has_flag(parent_->GetFlex(), EFlex::Vertical))
		{
			displaySettings.bounds.x = 0.0f;
		}
	}
	return displaySettings;
}

EFlex WSpacer::GetFlex() const
{
	if (parent_)
	{
		return parent_->GetFlex();
	}
	return EFlex::None;
}

#include "generated/Spacer.generated.inl"
