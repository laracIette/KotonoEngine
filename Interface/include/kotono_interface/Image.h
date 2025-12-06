#pragma once
#include "Widget.h"
#include <filesystem>
struct KtInterfaceRenderableProxy;
class WImage : public WWidget
{
public:
	struct ImageSettings
	{
		/// default = ""
		std::filesystem::path path{ "" };
	};

	/// Display an image over the widget's bounds
	WImage(const ImageSettings& imageSettings);

	void Cleanup() override;

protected:
	ImageSettings imageSettings_;
	KtInterfaceRenderableProxy* imageProxy_;

	void DisplayInternal(DisplaySettings displaySettings) override;
};

