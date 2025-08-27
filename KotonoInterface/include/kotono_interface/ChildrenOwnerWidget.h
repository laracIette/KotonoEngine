#pragma once
#include "Widget.h"
#include <vector>
class WChildrenOwnerWidget : public WWidget
{
	using WidgetVector = std::vector<WWidget*>;

public:
	WChildrenOwnerWidget(const WidgetVector& children);

	void Cleanup() override;

private:
	WidgetVector children_;
};

