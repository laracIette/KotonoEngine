#pragma once
#include "Widget.h"
class WChildOwnerWidget : public WWidget
{
public:
	WChildOwnerWidget(WWidget* child);

	void CacheBuild() override final;

	void Cleanup() override;

	EFlex GetFlex() const override;

	WidgetVector GetWidgetTree() override;

private:
	WWidget* child_;
};

