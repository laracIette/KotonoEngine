#pragma once
#include "View.h"
class WCenter;
class VCenterView : public VView
{
public:
	VCenterView(WCenter* center);

	void Build(UBuildSettings buildSettings) override;

private:
	WCenter* center_;
};

