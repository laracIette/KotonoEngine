#pragma once
#include "generated/Stack.generated.h"
#include "ChildrenOwner.h"
/// Display widgets on top of each other
class WStack final : public WChildrenOwner
{
	GENERATED_WSTACK()

public:
	glm::vec2 GetContentSize(glm::vec2 const& bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};

