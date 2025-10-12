#pragma once
#include "ChildrenOwnerWidget.h"
class WListBody : public WChildrenOwnerWidget
{
public:
	struct ListBodySettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		WidgetVector children{};
	};

	/// Defines a vertical container for widgets
	WListBody(const ListBodySettings& listBodySettings);

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;

protected:
	ListBodySettings listBodySettings_;

	void DisplayInternal(DisplaySettings displaySettings) override;
};
