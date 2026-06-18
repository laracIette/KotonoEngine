#include "Offset.h"

UWidgetDisplaySettings WOffset::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.position += offset_;
	
	if (child_)
	{
		return child_->GetContentDisplaySettings(displaySettings);
	}
	return displaySettings;
}

#include "generated/Offset.generated.inl"
