#pragma once
#include "ChildOwnerWidget.h"
class WWrap : public WChildOwnerWidget
{
public:
	enum class Axis : u8
	{
		Horizontal = 0x01,
		Vertical = 0x02,
		All = Horizontal | Vertical
	};

	struct WrapSettings
	{
		/// default = EFlex::All
		Axis axis{ Axis::All };
		WWidget* child{ nullptr };
	};

	/// Fills the entirety of the available parent space
	WWrap(const WrapSettings& wrapSettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;

protected:
	WrapSettings wrapSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};