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

	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetExpand() const -> EExpand override;
	auto GetFlex() const -> EFlex override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(EAxis, axis_, Axis);
	StateProperty(f32, size_, Size);
};

