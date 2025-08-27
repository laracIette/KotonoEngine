#include "BoundsView.h"
#include "Bounds.h"

VBoundsView::VBoundsView(WBounds* bounds) :
	VView(bounds)
{
}

void VBoundsView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	VView::Build(buildSettings);

	auto* asBounds{ static_cast<WBounds*>(widget_) };

	buildSettings.bounds = asBounds->boundsSettings_.size;

	if (asBounds->boundsSettings_.child)
	{
		++buildSettings.layer;
		if (asBounds->boundsSettings_.child)
		{
			asBounds->boundsSettings_.child->CreateView()->Build(buildSettings);
		}
	}
}
