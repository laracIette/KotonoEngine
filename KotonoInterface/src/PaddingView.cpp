#include "PaddingView.h"
#include "Padding.h"

VPaddingView::VPaddingView(WPadding* padding) :
	VView(padding),
	padding_(padding)
{
}

void VPaddingView::Build(UBuildSettings buildSettings)
{
	if (!padding_)
	{
		return;
	}

	VView::Build(buildSettings);

	buildSettings.bounds.x -= padding_->paddingSettings_.padding.l;
	buildSettings.bounds.x -= padding_->paddingSettings_.padding.r;
	buildSettings.bounds.y -= padding_->paddingSettings_.padding.t;
	buildSettings.bounds.y -= padding_->paddingSettings_.padding.b;

	buildSettings.position.x += (padding_->paddingSettings_.padding.l - padding_->paddingSettings_.padding.r) / 2.0f;
	buildSettings.position.y += (padding_->paddingSettings_.padding.t - padding_->paddingSettings_.padding.b) / 2.0f;

	if (padding_->paddingSettings_.child)
	{
		++buildSettings.layer;
		padding_->paddingSettings_.child->CreateView()->Build(buildSettings);
	}
}
