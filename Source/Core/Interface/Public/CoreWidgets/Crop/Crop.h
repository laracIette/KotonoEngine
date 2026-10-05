#pragma once
#include "Crop.generated.h"
#include "ChildOwner/ChildOwner.h"

#include <Padding.h>

class WCrop final : public WChildOwner
{
	GENERATED_WCROP()

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(UPadding, padding_, Padding, Value);
};