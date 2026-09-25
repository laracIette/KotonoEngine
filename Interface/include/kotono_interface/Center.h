#pragma once
#include "generated/Center.generated.h"
#include "ChildOwner.h"
/// Center the child widget on an Axis
class WCenter final : public WChildOwner
{
	GENERATED_WCENTER()

public:
	WCenter(const EAxis axis = EAxis::All);

public:
	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetFlex() const -> EFlex override;
	auto GetExpand() const -> EExpand override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(EAxis, axis_, Axis);
};

