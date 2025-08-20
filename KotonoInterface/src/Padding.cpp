#include "Padding.h"

WPadding::WPadding(const WidgetSettings& widgetSettings, const PaddingSettings& paddingSettings) :
	Base(widgetSettings),
	paddingSettings_(paddingSettings)
{
}

void WPadding::Build(BuildSettings buildSettings)
{
	buildSettings.bounds -= paddingSettings_.padding;

	if (paddingSettings_.child)
	{
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