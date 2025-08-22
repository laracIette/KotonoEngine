#include "Offset.h"
#include "OffsetView.h"

WOffset::WOffset(const OffsetSettings& offsetSettings) :
	WChildOwnerWidget(offsetSettings.child),
	offsetSettings_(offsetSettings)
{
}

VView* WOffset::CreateView()
{
	return new VOffsetView(this);
}

void WOffset::Destroy()
{
	if (offsetSettings_.child)
	{
		offsetSettings_.child->Destroy();
	}

	WWidget::Destroy();
}