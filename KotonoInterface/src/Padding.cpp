#include "Padding.h"

WPadding::WPadding(const PaddingSettings& paddingSettings) :
	WChildOwnerWidget(paddingSettings.child),
	paddingSettings_(paddingSettings)
{
}

void WPadding::Display(BuildSettings buildSettings)
{
	SetBuildSettings(buildSettings);

	buildSettings.bounds.x -= paddingSettings_.padding.l;
	buildSettings.bounds.x -= paddingSettings_.padding.r;
	buildSettings.bounds.y -= paddingSettings_.padding.t;
	buildSettings.bounds.y -= paddingSettings_.padding.b;

	buildSettings.position.x += (paddingSettings_.padding.l - paddingSettings_.padding.r) / 2.0f;
	buildSettings.position.y += (paddingSettings_.padding.t - paddingSettings_.padding.b) / 2.0f;

	++buildSettings.layer;

	if (paddingSettings_.child)
	{
		paddingSettings_.child->Display(buildSettings);
	}
}