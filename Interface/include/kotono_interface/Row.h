#pragma once
#include "ChildrenOwnerWidget.h"
class WRow : public WChildrenOwnerWidget
{
public:
	struct RowSettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		WidgetVector children{};
	};

	/// Defines an horizontal container for widgets
	WRow(const RowSettings& rowSettings);	

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;

protected:
	RowSettings rowSettings_;
	
	void DisplayInternal(DisplaySettings displaySettings) override;

private:
	size GetFlexCount() const;
};

