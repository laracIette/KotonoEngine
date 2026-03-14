#pragma once
#include "Widget.h"
#include <kotono_common/Buildable.h>
#include <kotono_common/Event.h>
class WScrollable : public WWidget
{
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