#pragma once
#include "ChildOwner/ChildOwner.h"

#include <Padding.h>

#include "Crop.generated.h"

class WCrop final : public WChildOwner
{
	GENERATED()

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(UPadding, padding_, Padding, Value);
};