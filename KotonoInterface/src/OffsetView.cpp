#include "OffsetView.h"
#include "Offset.h"

VOffsetView::VOffsetView(WOffset* offset) :
	VView(offset),
	offset_(offset)
{
}

void VOffsetView::Build(UBuildSettings buildSettings)
{
	if (!offset_)
	{
		return;
	}

	buildSettings.position = offset_->offsetSettings_.size; // todo: sure before ?

	VView::Build(buildSettings);

	if (offset_->offsetSettings_.child)
	{
		++buildSettings.layer;
		offset_->offsetSettings_.child->CreateView()->Build(buildSettings);
	}
}
