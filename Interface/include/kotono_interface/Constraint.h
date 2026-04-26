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

public:
	EAxis GetAxis() const;
	float GetSize() const;

	void SetAxis(const EAxis axis);
	void SetSize(const float size);

private:
	EAxis axis_;
	float size_;
};

