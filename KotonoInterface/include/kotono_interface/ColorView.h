#pragma once
#include "View.h"
class WColor;
class VColorView : public VView
{
public:
	VColorView(WColor* color);

	void Build(UBuildSettings buildSettings) override;
};

