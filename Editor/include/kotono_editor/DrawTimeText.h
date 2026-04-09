#pragma once
#include "generated/DrawTimeText.generated.h"
#include <kotono_interface/Widget.h>
class WDrawTimeText : public WWidget
{
	GENERATED_WDRAWTIMETEXT()

public:
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void UpdateText();
};

