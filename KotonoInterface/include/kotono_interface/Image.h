#pragma once
#include "Widget.h"
#include <filesystem>
struct KtRenderable2DProxy;
class WImage : public WWidget
{
	friend class VImageView;

public:
	struct ImageSettings
	{
		/// default = ""
		std::filesystem::path path{ "" };
	};

	/// Display an image over the widget's bounds
	WImage(const ImageSettings& imageSettings);

	void Display(BuildSettings buildSettings) override;

	void Cleanup() override;

protected:
	ImageSettings imageSettings_;
	KtRenderable2DProxy* imageProxy_;
};

