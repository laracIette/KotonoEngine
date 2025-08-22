#pragma once
#include "View.h"
class WOffset;
class VOffsetView : public VView
{
public:
	VOffsetView(WOffset* offset);

	void Build(UBuildSettings buildSettings) override;

private:
	WOffset* offset_;
};

