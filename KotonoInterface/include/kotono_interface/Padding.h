#pragma once
#include "Widget.h"
class WPadding : public WWidget
{
public:
	struct Padding
	{
		float l, t, r, b;
	};

	struct PaddingSettings
	{
		Padding padding{ 0.0f, 0.0f, 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	WPadding(const PaddingSettings& paddingSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	PaddingSettings paddingSettings_;
};

