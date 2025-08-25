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