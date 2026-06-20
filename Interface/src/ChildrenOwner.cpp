#include "ChildrenOwner.h"
#include <algorithm>
#include <ranges>
#include <kotono_common/enum_utils.h>		 

WChildrenOwner::~WChildrenOwner()
{
	for (auto& child : children_)
	{
		if (child)
		{
			child->Delete();
		}
	}
}

void WChildrenOwner::Remove()
{
	Base::Remove();

	for (auto& child : children_)
	{
		if (child)
		{
			child->Remove();
		}
	}
}

WidgetVector WChildrenOwner::WidgetTree() const
{
	WidgetVector result{ Base::WidgetTree() };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto sub{ child->WidgetTree() };
			result.insert(result.end(), sub.begin(), sub.end());
		}
	}

	return result;
}

const WidgetPool& WChildrenOwner::GetChildren() const
{
	return children_;
}

void WChildrenOwner::SetChildren(const WidgetPool& widgets)
{
	SetState([this, widgets]() {
		for (auto& child : children_)
		{
			if (child)
			{
				child->SetParent({});
			}
		}

		children_ = widgets;

		for (auto& child : children_)
		{
			if (child)
			{
				child->SetParent(Ptr());
			}
		}
	});
}

size WChildrenOwner::GetValidChildrenCount() const
{
	return std::count_if(children_.begin(), children_.end(),
		[](const WidgetPtr& child) { return child; }
	);
}

UChildrenOwnerTree::UChildrenOwnerTree(const UPtr<WChildrenOwner>& widget, const std::vector<UWidgetTree*>& children)
	: widget_(widget)
	, children_(children)
{
}

UChildrenOwnerTree::~UChildrenOwnerTree()
{
	for (const auto* widgetTree : children_)
	{
		delete widgetTree;
	}
}

WidgetPtr UChildrenOwnerTree::Widget() const
{
	return widget_;
}

void UChildrenOwnerTree::Link() const
{
	for (const auto* child : children_)
	{
		if (child)
		{
			child->Link();
		}
	}

	if (widget_)
	{
		WidgetPool widgets{};
		std::ranges::copy(children_
			| std::views::filter([](const UWidgetTree* child) { return child != nullptr; })
			| std::views::transform([](const UWidgetTree* child) { return child->Widget(); })
			, std::back_inserter(widgets)
		);
		widget_->SetChildren(widgets);
	}
}

#include "generated/ChildrenOwner.generated.inl"
