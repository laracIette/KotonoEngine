#include "ChildOwnerWidget.h"

WChildOwnerWidget::WChildOwnerWidget(WWidget* child) :
	child_(child)
{
	if (child_)
	{
		child_->SetParent(this);
	}
}

void WChildOwnerWidget::Cleanup()
{
	if (child_)
	{
		child_->Cleanup();
	}
	delete child_;

	WWidget::Cleanup();
}
