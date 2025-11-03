#pragma once
#include <kotono_interface/Widget.h>
class WUpdateTimeText : public WWidget
{
public:
	WWidget* Build() override;
	void Cleanup() override;

private:
	void UpdateText();
};

