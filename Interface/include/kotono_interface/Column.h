#pragma once
#include "generated/Column.generated.h"
#include "ChildrenOwner.h"
/// Defines a vertical container for widgets
class WColumn final : public WChildrenOwner
{
	GENERATED_WCOLUMN()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	size GetExpandCount() const;

private:
	WritableProperty(float, spacing_, Spacing);
};

