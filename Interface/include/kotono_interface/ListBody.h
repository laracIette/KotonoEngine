#pragma once
#include "generated/ListBody.generated.h"
#include "ChildrenOwner.h"
class WListBody : public WChildrenOwner
{
	GENERATED_WLISTBODY()

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
