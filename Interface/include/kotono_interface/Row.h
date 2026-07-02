#pragma once
#include "generated/Row.generated.h"
#include "ChildrenOwner.h"
/// Defines an horizontal container for widgets
class WRow final : public WChildrenOwner
{
	GENERATED_WROW()

public:
	WRow(const f32 spacing = 0.0f);

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;
	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:	
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	size GetExpandCount() const;

private:
	StateProperty(f32, spacing_, Spacing);
};

