#pragma once
#include "Widget.h"
#include <vector>
class WChildrenOwnerWidget : public WWidget
{
public:
	WChildrenOwnerWidget(const WidgetVector& children);

	void CacheBuild() override final;

	void Cleanup() override;

	EFlex GetFlex() const override;

	WidgetVector GetWidgetTree() override;

private:
	WidgetVector children_;
};

