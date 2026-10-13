#pragma once
#include "ChildrenOwner/ChildrenOwner.h"

#include "Column.generated.h"

/// Defines a vertical container for widgets
class WColumn final : public WChildrenOwner
{
	GENERATED()

public:
	WColumn();
	
	void Display(UWidgetDisplaySettings displaySettings) override;
	
	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetExpand() const -> EExpand override;
	auto GetFlex() const -> EFlex override;

protected:	

private:
	auto GetChildrenExpandWeight() const -> f32;

private:
	StateProperty(f32, spacing_, Spacing, Value);
};

