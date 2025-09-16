#pragma once
#include <kotono_interface/Widget.h>
class KTimer;
class WDrawTimeText : public WWidget
{
public:
	WWidget* Build() override;

	void Cleanup() override;

private:
	KTimer* updateTimer_;

	void UpdateText();
};

