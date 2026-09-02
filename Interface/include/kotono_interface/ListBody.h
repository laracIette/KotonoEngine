#pragma once
#include "generated/ListBody.generated.h"
#include "ChildrenOwner.h"
class WListBody final : public WChildrenOwner
{
	GENERATED_WLISTBODY()

public:
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(float, spacing_, Spacing);
};
