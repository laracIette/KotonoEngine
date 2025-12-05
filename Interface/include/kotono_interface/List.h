#pragma once
#include "Widget.h"
#include <kotono_common/Buildable.h>
class WList : public WWidget
{
	friend class WListBody;

public:
	struct ListSettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		KtBuildable<WidgetVector> children;
	};

	/// Defines a vertical container for widgets
	WList(const ListSettings& listSettings);

	WWidget* Build() override;

protected:
	ListSettings listSettings_;
	float offset_;
};

