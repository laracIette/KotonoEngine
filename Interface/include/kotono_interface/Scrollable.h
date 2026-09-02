#pragma once
#include "generated/Scrollable.generated.h"
#include "ChildOwner.h"
/// Makes the child of this widget scrollable while cropping the overflowing content
class WScrollable final : public WChildOwner
{
	GENERATED_WSCROLLABLE()

public:
	WScrollable(EAxis axis = EAxis::All);

	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

	glm::vec2 GetContentSize(glm::vec2 bounds) const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	void Scroll(glm::vec2 const& delta);

private:
	WritableProperty(EAxis, axis_, Axis);
	StateProperty(glm::vec2, offset_, Offset);
};