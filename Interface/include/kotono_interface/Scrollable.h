#pragma once
#include "Widget.h"
#include <kotono_common/Buildable.h>
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

private:
	ScrollableSettings scrollableSettings_;
	float offset_;
};