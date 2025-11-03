#pragma once
#include <kotono_interface/Widget.h>
class WDrawTimeText : public WWidget
{
public:
	WWidget* Build() override;
	void Cleanup() override;

private:
	void UpdateText();
};

