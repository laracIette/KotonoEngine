#include "ChildrenOwnerWidget.h"
#include "Expanded.h"

WChildrenOwnerWidget::WChildrenOwnerWidget(const WidgetVector& children) :
	children_(children)
{
	for (auto* child : children_)
	{
		if (child)
		{
			child->SetParent(this);
		}
	}
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

size_t WChildrenOwnerWidget::GetExpandedCount() const
{
	size_t expandedCount{ 0 };
	for (const auto* child : children_)
	{
		if (dynamic_cast<const WExpanded*>(child))
		{
			++expandedCount;
		}
	}
	return expandedCount;
}
