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
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(EAxis, axis_, Axis);
};

