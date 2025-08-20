#include "Padding.h"

WPadding::WPadding(const PaddingSettings& paddingSettings) :
	paddingSettings_(paddingSettings)
{
}

void WPadding::Build(BuildSettings buildSettings)
{
	WWidget::Build(buildSettings);

	buildSettings.bounds -= paddingSettings_.padding;

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