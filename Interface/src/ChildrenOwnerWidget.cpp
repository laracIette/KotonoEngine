#include "ChildrenOwnerWidget.h"
#include <algorithm>
#include <kotono_common/bitwise_utils.h>

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

void WChildrenOwnerWidget::CacheBuild()
{
	WWidget::CacheBuild();
	for (auto* child : children_)
	{
		if (child)
		{
			child->CacheBuild();
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
	children_.clear();

	WWidget::Cleanup();
}

EFlex WChildrenOwnerWidget::GetFlex() const
{
	EFlex result{ EFlex::None };

	for (auto* child : children_)
	{
		if (child)
		{
			result = bitwise_or(result, child->GetFlex());
		}
	}

	return result;
}

WWidget::WidgetVector WChildrenOwnerWidget::GetWidgetTree()
{
	WidgetVector result{ this };

	for (auto* child : children_)
	{
		if (child)
		{
			const auto sub{ child->GetWidgetTree() };
			result.insert(result.end(), sub.begin(), sub.end());
		}
	}

	return result;
}

size WChildrenOwnerWidget::GetValidChildrenCount() const
{
	return std::count_if(children_.begin(), children_.end(),
		[](const WidgetPtr child) { return child != nullptr; }
	);
}
