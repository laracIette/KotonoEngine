#pragma once
#include "View.h"
class WColumn;
class VColumnView : public VView
{
public:
	VColumnView(WColumn* widget);

	void Build(UBuildSettings buildSettings) override;
};

