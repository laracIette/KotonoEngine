#pragma once
#include "View.h"
class WBounds;
class VBoundsView : public VView
{
public:
	VBoundsView(WBounds* bounds);

	void Build(UBuildSettings buildSettings) override;
};

