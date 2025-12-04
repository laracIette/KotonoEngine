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

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;

protected:
	ColumnSettings columnSettings_;

	void DisplayInternal(DisplaySettings displaySettings) override;

private:
	size_t GetFlexCount() const;
};

