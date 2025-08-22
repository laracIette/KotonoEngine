#include "Padding.h"
#include "PaddingView.h"

WPadding::WPadding(const PaddingSettings& paddingSettings) :
	WChildOwnerWidget(paddingSettings.child),
	paddingSettings_(paddingSettings)
{
}

VView* WPadding::CreateView()
{
	return new VPaddingView(this);
}

void WPadding::Destroy()
{
	if (paddingSettings_.child)
	{
		paddingSettings_.child->Destroy();
	}

	WWidget::Destroy();
}