#include "SecondaryWindow/SecondaryWindow.h"

WSecondaryWindow::WSecondaryWindow(WidgetPtr const& widget)
	: widget_{ widget }
{
}

WidgetPtr WSecondaryWindow::Build()
{
	return widget_;
}

#include "SecondaryWindow.generated.inl"
