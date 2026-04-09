#pragma once
#include "generated/List.generated.h"
#include "Widget.h"
#include <kotono_common/Buildable.h>
class WList : public WWidget
{
	GENERATED_WLIST()

	friend class WListBody;

public:
	struct ListSettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		KtBuildable<WidgetVector> children{};
	};

	/// Defines a vertical container for widgets
	WList(const ListSettings& listSettings);

	WidgetPtr Build() override;

protected:
	ListSettings listSettings_;
};

