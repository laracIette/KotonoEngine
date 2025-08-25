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

WWidget* WChildrenOwnerWidget::GetDirty()
{
	if (isDirty_)
	{
		return this;
	}

	for (auto* child : children_)
	{
		if (child)
		{
			if (auto* dirty = child->GetDirty(); dirty)
			{
				return dirty;
			}
		}
	}

	return nullptr;
}
