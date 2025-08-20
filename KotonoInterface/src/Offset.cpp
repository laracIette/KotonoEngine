#include "Offset.h"

WOffset::WOffset(OffsetSettings offsetSettings) :
	offsetSettings_(offsetSettings)
{
}

void WOffset::Build(BuildSettings buildSettings)
{
	buildSettings.position = offsetSettings_.size;
	WWidget::Build(buildSettings);

	if (offsetSettings_.child)
	{
		++buildSettings.layer;
		offsetSettings_.child->Build(buildSettings);
	}
}

void WOffset::Destroy()
{
	if (offsetSettings_.child)
	{
		offsetSettings_.child->Destroy();
	}
}