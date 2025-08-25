#include "Bounds.h"
#include "BoundsView.h"

WBounds::WBounds(const BoundsSettings& boundsSettings) :
	WChildOwnerWidget(boundsSettings.child),
	boundsSettings_(boundsSettings)
{
}

VView* WBounds::CreateView()
{
	return new VBoundsView(this);
}
