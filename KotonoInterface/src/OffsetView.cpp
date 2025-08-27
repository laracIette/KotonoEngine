#include "OffsetView.h"
#include "Offset.h"

VOffsetView::VOffsetView(WOffset* offset) :
	VView(offset)
{
}

void VOffsetView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	auto* asOffset{ static_cast<WOffset*>(widget_) };

	VView::Build(buildSettings);

	buildSettings.position = asOffset->offsetSettings_.size;

	if (asOffset->offsetSettings_.child)
	{
		++buildSettings.layer;
		asOffset->offsetSettings_.child->CreateView()->Build(buildSettings);
	}
}
