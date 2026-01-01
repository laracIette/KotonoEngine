#pragma once
#include "Widget.h"
#include <kotono_common/Path.h>
class UInterfaceProxy;
class WImage : public WWidget
{
public:
	struct ImageSettings
	{
		/// default = ""
		UPath path{ "" };
	};

	/// Display an image over the widget's bounds
	WImage(const ImageSettings& imageSettings);

	void Cleanup() override;

protected:
	ImageSettings imageSettings_;
	UInterfaceProxy* imageProxy_;

	void DisplayInternal(DisplaySettings displaySettings) override;
};

