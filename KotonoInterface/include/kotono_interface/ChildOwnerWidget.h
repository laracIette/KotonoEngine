#pragma once
#include "Widget.h"
class WChildOwnerWidget : public WWidget
{
public:
	WChildOwnerWidget(WWidget* child);

	void Cleanup() override;

	WWidget* GetDirty() override;

private:
	WWidget* child_;
};

