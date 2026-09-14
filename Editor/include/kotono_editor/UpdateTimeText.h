#pragma once
#include "generated/UpdateTimeText.generated.h"
#include <kotono_core/Widget.h>
class WUpdateTimeText : public WWidget
{
	GENERATED_WUPDATETIMETEXT()

protected:
	WidgetPtr Build() override;
};

