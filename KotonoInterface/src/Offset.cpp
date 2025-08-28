#include "Offset.h"

WOffset::WOffset(const OffsetSettings& offsetSettings) :
	WChildOwnerWidget(offsetSettings.child),
	offsetSettings_(offsetSettings)
{
}

void WOffset::Display(BuildSettings buildSettings)
{
	SetBuildSettings(buildSettings);

	buildSettings.position = offsetSettings_.size;
	++buildSettings.layer;

	if (offsetSettings_.child)
	{
		offsetSettings_.child->Display(buildSettings);
	}
}