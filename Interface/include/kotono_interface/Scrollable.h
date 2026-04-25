#pragma once
#include "generated/Scrollable.generated.h"
#include "ChildOwner.h"
#include <kotono_common/Event.h>
/// Makes the child of this widget scrollable while cropping the overflowing content
class WScrollable final : public WChildOwner
{
	GENERATED_WSCROLLABLE()

public:
	WidgetPtr Build() override;

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

public:
	EAxis GetAxis() const;

	void SetAxis(const EAxis axis);

private:
	void Scroll(const float delta);

	UEvent<float>& GetScrollEvent() const;

private:
	EAxis axis_;

	float offset_;
};