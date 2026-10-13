#pragma once
#include "ChildrenOwner/ChildrenOwner.h"

#include "Stack.generated.h"

/// Display widgets on top of each other
class WStack final : public WChildrenOwner
{
	GENERATED()

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	
	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetExpand() const -> EExpand override;
	auto GetFlex() const -> EFlex override;
};

