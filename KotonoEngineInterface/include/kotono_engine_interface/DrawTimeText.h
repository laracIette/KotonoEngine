#pragma once
#include <kotono_interface/Widget.h>
#include <kotono_engine/Ptr.h>
class KTimer;
class WDrawTimeText : public WWidget
{
public:
	WWidget* Build() override;

	void Cleanup() override;

private:
	UPtr<KTimer> updateTimer_;

	void UpdateText();
};

