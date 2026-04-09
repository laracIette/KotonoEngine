#pragma once
#include "generated/ChildOwner.generated.h"
#include "Widget.h"
class WChildOwner : public WWidget
{
	GENERATED_WCHILDOWNER()

public:
	void CacheBuild() override final;

	void Cleanup() override;

	EFlex GetFlex() const override;
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

	WidgetVector GetWidgetTree() override;

public:
	const WidgetPtr& GetChild() const;

	void SetChild(const WidgetPtr& widget);

protected:
	WidgetPtr child_;
};
