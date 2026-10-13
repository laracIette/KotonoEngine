#pragma once
#include "ChildrenOwner/ChildrenOwner.h"

#include "Row.generated.h"

/// Defines an horizontal container for widgets
class WRow final : public WChildrenOwner
{
	GENERATED()

public:
	WRow();
	
	void Display(UWidgetDisplaySettings displaySettings) override;
	
	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetExpand() const -> EExpand override;
	auto GetFlex() const -> EFlex override;

private:
	auto GetChildrenExpandWeight() const -> f32;

private:
	StateProperty(f32, spacing_, Spacing, Value);
};

