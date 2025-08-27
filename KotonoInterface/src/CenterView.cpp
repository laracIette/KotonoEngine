#include "CenterView.h"
#include "Center.h"

VCenterView::VCenterView(WCenter* center) :
	VView(center)
{
}

void VCenterView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	VView::Build(buildSettings);

	auto* asCenter{ static_cast<WCenter*>(widget_) };

	switch (asCenter->centerSettings_.axis)
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

	if (asCenter->centerSettings_.child)
	{
		++buildSettings.layer;
		asCenter->centerSettings_.child->CreateView()->Build(buildSettings);
	}
}
