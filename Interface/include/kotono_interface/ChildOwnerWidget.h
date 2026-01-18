#pragma once
#include "Widget.h"
class WChildOwnerWidget : public WWidget
{
public:
	WChildOwnerWidget(WWidget* child);

	void CacheBuild() override final;

	void Cleanup() override;

	EFlex GetFlex() const override;
	glm::vec2 GetDesiredSize() const override;

	WidgetVector GetWidgetTree() override;

private:
	WWidget* child_;
};

