#include "Center.h"
#include "CenterView.h"

WCenter::WCenter(const CenterSettings& centerSettings) :
	WChildOwnerWidget(centerSettings.child),
	centerSettings_(centerSettings)
{
}

VView* WCenter::CreateView()
{
	return new VCenterView(this);
}

void WCenter::Destroy()
{
	if (centerSettings_.child)
	{
		centerSettings_.child->Destroy();
	}

	WWidget::Destroy();
}
