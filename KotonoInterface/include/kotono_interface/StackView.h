#pragma once
#include "View.h"
class WStack;
class VStackView : public VView
{
public:
	VStackView(WStack* stack);

	void Build(UBuildSettings buildSettings) override;

private:
	WStack* stack_;
};

