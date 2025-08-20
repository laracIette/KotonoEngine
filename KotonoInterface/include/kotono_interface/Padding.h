#pragma once
#include "Widget.h"
class WPadding : public WWidget
{
	using Base = WWidget;

public:
	struct PaddingSettings
	{
		glm::vec2 padding;
		WWidget* child;
	};

	WPadding(const WidgetSettings& widgetSettings, const PaddingSettings& paddingSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	PaddingSettings paddingSettings_;
};

