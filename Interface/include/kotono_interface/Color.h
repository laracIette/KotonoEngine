#pragma once
#include "Widget.h"
#include <kotono_graphics/Color.h>
class UInterfaceProxy;
class WColor : public WWidget
{
public:
	struct ColorSettings
	{
		/// default = UColor::White()
		UColor color{ UColor::White() };
	};

	/// Fill the widget's bounds with a color
	WColor(const ColorSettings& colorSettings);

	void Cleanup() override;
	
protected:
	ColorSettings colorSettings_;
	UInterfaceProxy* colorProxy_;

	void DisplayInternal(DisplaySettings displaySettings) override;
};

