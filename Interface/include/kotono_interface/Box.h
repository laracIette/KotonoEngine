#pragma once
#include "generated/Box.generated.h"
#include "ChildOwner.h"
/// Set bounds for the child widget to be contained within
class WBox final : public WChildOwner
{
	GENERATED_WBOX()

public:
	WBox(glm::vec2 const& size = { 64.0f, 64.0f });

public:
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(glm::vec2, size_, Size);
};

