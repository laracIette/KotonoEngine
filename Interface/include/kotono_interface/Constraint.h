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

	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(EAxis, axis_, Axis);
	StateProperty(f32, size_, Size);
};

