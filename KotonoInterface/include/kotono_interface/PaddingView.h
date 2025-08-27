#pragma once
#include "View.h"
class WPadding;
class VPaddingView : public VView
{
public:
	VPaddingView(WPadding* padding);

	void Build(UBuildSettings buildSettings) override;
};

