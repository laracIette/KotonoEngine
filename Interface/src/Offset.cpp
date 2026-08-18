#include "Offset.h"

UWidgetDisplaySettings WOffset::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.position += offset_;
	
	if (GetChild())
	{
		return GetChild()->GetContentDisplaySettings(displaySettings);
	}
	return displaySettings;
}

#include "generated/Offset.generated.inl"
