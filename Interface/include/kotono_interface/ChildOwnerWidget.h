#pragma once
#include "Widget.h"
class WChildOwnerWidget : public WWidget
{
public:
	WChildOwnerWidget(WidgetPtr child);

	void CacheBuild() override final;

	void Cleanup() override;

	EFlex GetFlex() const override;
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

	WidgetVector GetWidgetTree() override;

private:
	WidgetPtr child_;
};

