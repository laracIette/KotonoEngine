#include "Bounds.h"

WBounds::WBounds(BoundsSettings boundsSettings) :
	boundsSettings_(boundsSettings)
{
}

void WBounds::Build(BuildSettings buildSettings)
{
	buildSettings.bounds = boundsSettings_.size;
	WWidget::Build(buildSettings);

	if (boundsSettings_.child)
	{
		++buildSettings.layer;
		boundsSettings_.child->Build(buildSettings);
	}
}

void WBounds::Destroy()
{
	if (boundsSettings_.child)
	{
		boundsSettings_.child->Destroy();
	}
}
