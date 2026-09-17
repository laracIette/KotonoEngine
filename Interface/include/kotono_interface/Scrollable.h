#pragma once
#include "generated/Scrollable.generated.h"
#include "ChildOwner.h"
/// Makes the child of this widget scrollable while cropping the overflowing content
class WScrollable final : public WChildOwner
{
	GENERATED_WSCROLLABLE()

public:
	WScrollable(EAxis axis = EAxis::All);

	b8 OnMouseScroll(glm::vec2 const& delta, glm::vec2 const& position) override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	WritableProperty(EAxis, axis_, Axis);
	StateProperty(glm::vec2, offset_, Offset);
};