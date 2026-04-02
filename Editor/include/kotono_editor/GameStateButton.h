#pragma once
#include <kotono_interface\Widget.h>
class WGameStateButton : public WWidget
{
public:
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void OnKeyboardSpaceKeyPressed();
};

