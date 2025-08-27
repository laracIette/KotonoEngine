#include "PaddingView.h"
#include "Padding.h"

VPaddingView::VPaddingView(WPadding* padding) :
	VView(padding)
{
}

void VPaddingView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	VView::Build(buildSettings);

	auto* asPadding{ static_cast<WPadding*>(widget_) };

	buildSettings.bounds.x -= asPadding->paddingSettings_.padding.l;
	buildSettings.bounds.x -= asPadding->paddingSettings_.padding.r;
	buildSettings.bounds.y -= asPadding->paddingSettings_.padding.t;
	buildSettings.bounds.y -= asPadding->paddingSettings_.padding.b;

	buildSettings.position.x += (asPadding->paddingSettings_.padding.l - asPadding->paddingSettings_.padding.r) / 2.0f;
	buildSettings.position.y += (asPadding->paddingSettings_.padding.t - asPadding->paddingSettings_.padding.b) / 2.0f;

	if (asPadding->paddingSettings_.child)
	{
		++buildSettings.layer;
		asPadding->paddingSettings_.child->CreateView()->Build(buildSettings);
	}
}
