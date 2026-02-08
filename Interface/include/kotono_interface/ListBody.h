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

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	ListBodySettings listBodySettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};
