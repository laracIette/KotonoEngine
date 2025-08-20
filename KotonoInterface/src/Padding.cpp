#include "Padding.h"

WPadding::WPadding(const PaddingSettings& paddingSettings) :
	paddingSettings_(paddingSettings)
{
}

void WPadding::Build(BuildSettings buildSettings)
{
	WWidget::Build(buildSettings);

	buildSettings.bounds.x -= paddingSettings_.padding.l;
	buildSettings.bounds.x -= paddingSettings_.padding.r;
	buildSettings.bounds.y -= paddingSettings_.padding.t;
	buildSettings.bounds.y -= paddingSettings_.padding.b;

	buildSettings.position.x += (paddingSettings_.padding.l - paddingSettings_.padding.r) / 2.0f;
	buildSettings.position.y += (paddingSettings_.padding.t - paddingSettings_.padding.b) / 2.0f;

	if (paddingSettings_.child)
	{
		++buildSettings.layer;
		paddingSettings_.child->Build(buildSettings);
	}
}

void WPadding::Destroy()
{
	if (paddingSettings_.child)
	{
		paddingSettings_.child->Destroy();
	}
}