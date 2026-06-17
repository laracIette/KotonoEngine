#pragma once
#include "generated/Box.generated.h"
#include "ChildOwner.h"
/// Set bounds for the child widget to be contained within
class WBox final : public WChildOwner
{
	GENERATED_WBOX()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

public:
	const glm::vec2& GetSize() const;

	void SetSize(const glm::vec2& size);

private:
	glm::vec2 size_;
};

