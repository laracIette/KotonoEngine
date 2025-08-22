#include "CenterView.h"
#include "Center.h"

VCenterView::VCenterView(WCenter* center) :
	VView(center),
	center_(center)
{
}

void VCenterView::Build(UBuildSettings buildSettings)
{
	if (!center_)
	{
		return;
	}

	VView::Build(buildSettings);

	switch (center_->centerSettings_.axis)
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

	if (center_->centerSettings_.child)
	{
		++buildSettings.layer;
		center_->centerSettings_.child->CreateView()->Build(buildSettings);
	}
}
