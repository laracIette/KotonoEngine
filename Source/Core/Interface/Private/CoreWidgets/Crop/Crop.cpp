#include "CoreWidgets/Crop/Crop.h"

void WCrop::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	displaySettings.scissor.offset = displaySettings.position;
	displaySettings.scissor.extent = displaySettings.bounds;
	
	displaySettings.scissor.offset.x += padding_.l;
	displaySettings.scissor.offset.y += padding_.t;

	displaySettings.scissor.extent.x -= padding_.l;
	displaySettings.scissor.extent.x -= padding_.r;
	displaySettings.scissor.extent.y -= padding_.t;
	displaySettings.scissor.extent.y -= padding_.b;

	Base::DisplayInternal(displaySettings);
}

#include "generated/Crop.generated.inl"
