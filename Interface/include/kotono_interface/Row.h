#pragma once
#include "ChildrenOwnerWidget.h"
class WRow : public WChildrenOwnerWidget
{
public:
	struct RowSettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		/// default = false
		bool shouldWrap{ true };
		WidgetVector children{};
	};

	/// Defines an horizontal container for widgets
	WRow(const RowSettings& rowSettings);	

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize() const override;

protected:
	RowSettings rowSettings_;
	
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	size GetFlexCount() const;

	std::vector<glm::vec2> GetRowDisplaySizes(const UWidgetDisplaySettings& displaySettings) const;
};

