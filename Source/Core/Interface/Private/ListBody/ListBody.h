#pragma once
#include "ChildrenOwner/ChildrenOwner.h"

#include "ListBody.generated.h"

class WListBody final : public WChildrenOwner
{
	GENERATED()

public:
	WListBody();
	
	void Display(UWidgetDisplaySettings displaySettings) override;
	
	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(const glm::vec2& bounds) const -> glm::vec2 override;

	auto GetFlex() const -> EFlex override;

private:
	StateProperty(float, spacing_, Spacing);
};
