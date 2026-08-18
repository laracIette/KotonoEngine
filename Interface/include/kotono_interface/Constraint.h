#pragma once
#include "generated/Constraint.generated.h"
#include "ChildOwner.h"
/// Constraints the bounds of the child widget given an axis
class WConstraint final : public WChildOwner
{
	GENERATED_WCONSTRAINT()

public:
	WConstraint(EAxis axis, f32 size);
	WConstraint();

	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;
	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

private:
	StateProperty(EAxis, axis_, Axis);
	StateProperty(f32, size_, Size);
};

