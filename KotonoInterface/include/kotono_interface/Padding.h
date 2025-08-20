#pragma once
#include "Widget.h"
class WPadding : public WWidget
{
public:
	struct PaddingSettings
	{
		glm::vec2 padding;
		WWidget* child;
	};

	WPadding(const PaddingSettings& paddingSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	PaddingSettings paddingSettings_;
};

