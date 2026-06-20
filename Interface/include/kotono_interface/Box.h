#pragma once
#include "generated/Box.generated.h"
#include "ChildOwner.h"
/// Set bounds for the child widget to be contained within
class WBox final : public WChildOwner
{
	GENERATED_WBOX()

public:
	WBox(const glm::vec2& size = { 64.0f, 64.0f });

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;
	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

private:
	StateProperty(glm::vec2, size_, Size);
};

