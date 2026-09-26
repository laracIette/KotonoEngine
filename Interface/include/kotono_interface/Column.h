#pragma once
#include "generated/Column.generated.h"
#include "ChildrenOwner.h"
/// Defines a vertical container for widgets
class WColumn final : public WChildrenOwner
{
	GENERATED_WCOLUMN()

public:
	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetExpand() const -> EExpand override;
	auto GetFlex() const -> EFlex override;

protected:	
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	auto GetExpandCount() const -> size;

private:
	StateProperty(f32, spacing_, Spacing, Value);
};

