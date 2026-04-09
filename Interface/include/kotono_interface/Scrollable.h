#pragma once
#include "generated/Scrollable.generated.h"
#include "Widget.h"
#include <kotono_common/Buildable.h>
#include <kotono_common/Event.h>
/// Makes the child of this widget scrollable with scissor over the bounds of this widget
class WScrollable : public WWidget
{
	GENERATED_WSCROLLABLE()

public:
	struct ScrollableSettings
	{
		/// default = EAxis::Vertical
		EAxis axis{ EAxis::Vertical };
		KtBuildable<WidgetPtr> child{};
	};

	WScrollable(const ScrollableSettings& scrollableSettings);

	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void Scroll(const float delta);

	UEvent<float>& GetScrollEvent() const;

private:
	ScrollableSettings scrollableSettings_;
	float offset_;
};