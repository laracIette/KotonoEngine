#pragma once
#include <kotono_interface/Widget.h>
class WDrawTimeText : public WWidget
{
public:
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void UpdateText();
};

