#pragma once
#include "ChildOwner/ChildOwner.h"

#include "Constraint.generated.h"

/// Constraints the bounds of the child widget given an axis
class WConstraint final : public WChildOwner
{
	GENERATED()

public:
	WConstraint();

	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetExpand() const -> EExpand override;
	auto GetFlex() const -> EFlex override;
	
protected:
	auto GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings override;

private:
	StateProperty(EAxis, axis_, Axis);
	StateProperty(f32, size_, Size);
};

