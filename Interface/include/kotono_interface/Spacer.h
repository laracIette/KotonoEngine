#pragma once
#include "Widget.h"
class WSpacer : public WWidget
{
public:
	struct SpacerSettings
	{
	};

	/// Fills the entirety of the available parent space
	WSpacer(const SpacerSettings& spacerSettings);

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;

private:
	SpacerSettings spacerSettings_;
};

