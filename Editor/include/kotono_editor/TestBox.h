#pragma once
#include <kotono_interface/Widget.h>
class WTestBox : public WWidget
{
public:
	WidgetPtr Build() override;
	void Cleanup() override;
};

