#include "Bounds.h"

WBounds::WBounds(const BoundsSettings& boundsSettings) :
	WChildOwnerWidget(boundsSettings.child),
	boundsSettings_(boundsSettings)
{
}

void WBounds::Display(BuildSettings buildSettings)
{
	SetBuildSettings(buildSettings);

	buildSettings.bounds = boundsSettings_.size;
	++buildSettings.layer;

	if (boundsSettings_.child)
	{
		boundsSettings_.child->Display(buildSettings);
	}
}
