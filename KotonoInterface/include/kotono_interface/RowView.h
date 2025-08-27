#pragma once
#include "View.h"
class WRow;
class VRowView : public VView
{
public:
	VRowView(WRow* row);

	void Build(UBuildSettings buildSettings) override;
};

