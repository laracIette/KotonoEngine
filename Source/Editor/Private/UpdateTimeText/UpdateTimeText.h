#pragma once
#include "generated/UpdateTimeText.generated.h"
#include <Widget/Widget.h>

class WUpdateTimeText : public WWidget
{
	GENERATED_WUPDATETIMETEXT()

protected:
	WidgetPtr Build() override;
};

