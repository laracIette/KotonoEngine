#pragma once
#include "ChildOwnerWidget.h"
#include "Axis.h"
class WCenter : public WChildOwnerWidget
{
public:
	struct CenterSettings
	{
		/// default  = Axis::All
		EAxis axis{ EAxis::All };
		WWidget* child{ nullptr };
	};

	/// Center the child widget on an Axis
	WCenter(const CenterSettings& centerSettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

protected:
	CenterSettings centerSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};

