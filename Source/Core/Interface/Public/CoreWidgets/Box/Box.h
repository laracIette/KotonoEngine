#pragma once
#include "ChildOwner/ChildOwner.h"

#include "Box.generated.h"

/// Set bounds for the child widget to be contained within
class WBox final : public WChildOwner
{
	GENERATED()

public:
	WBox();

	glm::vec2 GetContentSize(glm::vec2 const& bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:
	auto GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings override;

private:
	StateProperty(glm::vec2, size_, Size);
};

