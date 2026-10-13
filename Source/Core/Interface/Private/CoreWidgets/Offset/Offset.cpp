#include "CoreWidgets/Offset/Offset.h"

auto WOffset::GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings
{
	displaySettings.position += offset_;
	
	return displaySettings;
}

#include "Offset.generated.inl"
