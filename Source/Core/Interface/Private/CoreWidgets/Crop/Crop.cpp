#include "CoreWidgets/Crop/Crop.h"

WCrop::WCrop() 
	: padding_{ UPadding::Zero() }
{
}

auto WCrop::GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings
{	
	displaySettings.scissor.offset = displaySettings.position;
	displaySettings.scissor.extent = displaySettings.bounds;
	
	displaySettings.scissor.offset.x += padding_.l;
	displaySettings.scissor.offset.y += padding_.t;

	displaySettings.scissor.extent.x -= padding_.l;
	displaySettings.scissor.extent.x -= padding_.r;
	displaySettings.scissor.extent.y -= padding_.t;
	displaySettings.scissor.extent.y -= padding_.b;
	
	return displaySettings;
}

#include "Crop.generated.inl"
