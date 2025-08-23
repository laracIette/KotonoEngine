#include "ChildrenOwnerWidget.h"
#include "log.h"

WChildrenOwnerWidget::WChildrenOwnerWidget(const WidgetVector& children) :
	children_(children)
{
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
