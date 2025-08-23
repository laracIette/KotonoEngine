#include "ChildOwnerWidget.h"
#include "log.h"

WChildOwnerWidget::WChildOwnerWidget(WWidget* child) :
	child_(child)
{
}

WWidget* WChildOwnerWidget::GetDirty()
{
	if (isDirty_)
	{
		return this;
	}

	return child_->GetDirty();
}
