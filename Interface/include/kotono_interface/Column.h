#pragma once
#include "ChildrenOwnerWidget.h"
class WColumn : public WChildrenOwnerWidget
{
public:
	struct ColumnSettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		WidgetVector children{};
	};

	/// Defines a vertical container for widgets
	WColumn(const ColumnSettings& columnSettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	ColumnSettings columnSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	size GetFlexCount() const;
};

