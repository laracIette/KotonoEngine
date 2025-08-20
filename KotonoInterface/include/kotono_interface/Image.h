#pragma once
#include "Widget.h"
#include <filesystem>
struct KtRenderable2DProxy;
class WImage : public WWidget
{
	using Base = WWidget;

public:
	struct ImageSettings
	{
		glm::vec2 size{ 32.0f, 32.0f };
		std::filesystem::path path{ "" };
	};

	WImage(const WidgetSettings& widgetSettings, const ImageSettings& imageSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	ImageSettings imageSettings_;
	KtRenderable2DProxy* imageProxy_;
};

