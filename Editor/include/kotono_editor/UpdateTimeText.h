#pragma once
#include "generated/UpdateTimeText.generated.h"
#include <kotono_interface/Widget.h>
class WUpdateTimeText : public WWidget
{
	GENERATED_WUPDATETIMETEXT()

public:
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void UpdateText();
};

