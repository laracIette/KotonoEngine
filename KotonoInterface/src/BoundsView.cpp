#include "BoundsView.h"
#include "Bounds.h"

VBoundsView::VBoundsView(WBounds* bounds) :
	VView(bounds),
	bounds_(bounds)
{
}

void VBoundsView::Build(UBuildSettings buildSettings)
{
	if (!bounds_)
	{
		return;
	}

	buildSettings.bounds = bounds_->boundsSettings_.size; // todo: sure before ?

	VView::Build(buildSettings);

	if (bounds_->boundsSettings_.child)
	{
		++buildSettings.layer;
		if (bounds_->boundsSettings_.child)
		{
			bounds_->boundsSettings_.child->CreateView()->Build(buildSettings);
		}
	}
}
