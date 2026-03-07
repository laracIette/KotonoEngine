#pragma once
#include "ChildOwnerWidget.h"
class WWrap : public WChildOwnerWidget
{
public:
	struct WrapSettings
	{
		/// default = EAxis::All
		EAxis axis{ EAxis::All };
		WidgetPtr child{ nullptr };
	};

	/// Fills the entirety of the available parent space
	WWrap(const WrapSettings& wrapSettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;

protected:
	WrapSettings wrapSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};