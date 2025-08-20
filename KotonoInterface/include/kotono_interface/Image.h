#pragma once
#include "Widget.h"
#include <filesystem>
struct KtRenderable2DProxy;
class WImage : public WWidget
{
public:
	struct ImageSettings
	{
		std::filesystem::path path{ "" };
	};

	WImage(const ImageSettings& imageSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	ImageSettings imageSettings_;
	KtRenderable2DProxy* imageProxy_;
};

