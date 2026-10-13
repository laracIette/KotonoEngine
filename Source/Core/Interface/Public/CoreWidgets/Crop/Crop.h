#pragma once
#include "ChildOwner/ChildOwner.h"

#include <Padding.h>

#include "Crop.generated.h"

class WCrop final : public WChildOwner
{
	GENERATED()

public:
	WCrop();
	
protected:
	auto GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings override;

private:
	StateProperty(UPadding, padding_, Padding, Value);
};