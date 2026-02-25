#pragma once
#include <kotono_interface/Widget.h>
class WUpdateTimeText : public WWidget
{
public:
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void UpdateText();
};

