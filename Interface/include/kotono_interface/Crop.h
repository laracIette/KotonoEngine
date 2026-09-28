#pragma once
#include "generated/Crop.generated.h"
#include "ChildOwner.h"

#include <kotono_graphics/Padding.h>

class WCrop final : public WChildOwner
{
	GENERATED_WCROP()

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(UPadding, padding_, Padding, Value);
};