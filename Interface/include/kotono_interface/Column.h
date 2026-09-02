#pragma once
#include "generated/Column.generated.h"
#include "ChildrenOwner.h"
/// Defines a vertical container for widgets
class WColumn final : public WChildrenOwner
{
	GENERATED_WCOLUMN()

public:
	WColumn(f32 spacing = 0.0f);

public:
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	size GetExpandCount() const;

private:
	StateProperty(f32, spacing_, Spacing);
};

