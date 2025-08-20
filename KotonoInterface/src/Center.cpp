#include "Center.h"

WCenter::WCenter(const CenterSettings& centerSettings) :
	centerSettings_(centerSettings)
{
}

void WCenter::Build(BuildSettings buildSettings)
{
	WWidget::Build(buildSettings);

	switch (centerSettings_.axis)
	{
	case Axis::All:
		buildSettings.position = (buildSettings.position + buildSettings.bounds) / 2.0f;
		break;
	case Axis::Horizontal:
		buildSettings.position.x = (buildSettings.position.x + buildSettings.bounds.x) / 2.0f;
		break;
	case Axis::Vertical:
		buildSettings.position.y = (buildSettings.position.y + buildSettings.bounds.y) / 2.0f;
		break;
	}
	
	if (centerSettings_.child)
	{
		++buildSettings.layer;
		centerSettings_.child->Build(buildSettings);
	}
}

void WCenter::Destroy()
{
	if (centerSettings_.child)
	{
		centerSettings_.child->Destroy();
	}
}
