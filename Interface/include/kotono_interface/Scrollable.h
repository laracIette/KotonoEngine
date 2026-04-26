#pragma once
#include "generated/Scrollable.generated.h"
#include "ChildOwner.h"
#include <kotono_common/Event.h>
/// Makes the child of this widget scrollable while cropping the overflowing content
class WScrollable final : public WChildOwner
{
	GENERATED_WSCROLLABLE()

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

private:
	void Scroll(const glm::vec2 delta);

private:
	WritableProperty(EAxis, axis_, Axis);
	glm::vec2 offset_;
};