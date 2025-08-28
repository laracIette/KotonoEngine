#include "Center.h"

WCenter::WCenter(const CenterSettings& centerSettings) :
	WChildOwnerWidget(centerSettings.child),
	centerSettings_(centerSettings)
{
}

void WCenter::Display(BuildSettings buildSettings)
{
	SetBuildSettings(buildSettings);

	switch (centerSettings_.axis)
	{
	case WCenter::Axis::All:
		buildSettings.position = (buildSettings.position + buildSettings.bounds) / 2.0f;
		break;
	case WCenter::Axis::Horizontal:
		buildSettings.position.x = (buildSettings.position.x + buildSettings.bounds.x) / 2.0f;
		break;
	case WCenter::Axis::Vertical:
		buildSettings.position.y = (buildSettings.position.y + buildSettings.bounds.y) / 2.0f;
		break;
	}
	++buildSettings.layer;

	if (centerSettings_.child)
	{
		centerSettings_.child->Display(buildSettings);
	}
}
