#pragma once
#include "generated/Constraint.generated.h"
#include "ChildOwner.h"
/// Constraints the bounds of the child widget given an axis
class WConstraint final : public WChildOwner
{
	GENERATED_WCONSTRAINT()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

private:
	WritableProperty(EAxis, axis_, Axis);
	WritableProperty(float, size_, Size);
};

