#include "View.h"
#include "Widget.h"

VView::VView(WWidget* widget) :
	widget_(widget)
{
}

void VView::Build(UBuildSettings buildSettings)
{
	if (widget_)
	{
		widget_->position_ = buildSettings.position;
		widget_->size_ = buildSettings.bounds;
	}
}