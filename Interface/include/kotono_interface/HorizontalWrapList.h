#pragma once
#include "Widget.h"
#include <kotono_common/Buildable.h>
class WHorizontalWrapList : public WWidget
{
	friend class WHorizontalWrapListBody;

public:
	struct HorizontalWrapListSettings
	{
		/// default = 0.0f
		float itemSpacing{ 0.0f };
		/// default = 0.0f
		float rowSpacing{ 0.0f };
		KtBuildable<WidgetVector> children{};
	};

	/// Defines a vertical container for widgets
	WHorizontalWrapList(const HorizontalWrapListSettings& horizontalWrapListSettings);

	WidgetPtr Build() override;

protected:
	HorizontalWrapListSettings horizontalWrapListSettings_;
};