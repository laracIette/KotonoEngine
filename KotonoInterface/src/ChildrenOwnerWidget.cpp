#include "ChildrenOwnerWidget.h"

WChildrenOwnerWidget::WChildrenOwnerWidget(const WidgetVector& children) :
	children_(children)
{
}

void WChildrenOwnerWidget::Cleanup()
{
	for (auto* child : children_)
	{
		if (child)
		{
			child->Cleanup();
		}
		delete child;
	}

	WWidget::Cleanup();
}
