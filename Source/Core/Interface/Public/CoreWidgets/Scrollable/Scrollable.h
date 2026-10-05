#pragma once
#include "generated/Scrollable.generated.h"
#include "ChildOwner/ChildOwner.h"

/// Makes the child of this widget scrollable while cropping the overflowing content
class WScrollable final : public WChildOwner
{
	GENERATED_WSCROLLABLE()

public:
	WScrollable();

	b8 OnMouseScroll(glm::vec2 const& delta) override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	WritableProperty(EAxis, axis_, Axis);
	StateProperty(glm::vec2, offset_, Offset);
};