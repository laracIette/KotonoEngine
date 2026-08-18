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

void WChildrenOwner::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	for (auto& child : children_)
	{
		if (child && child->GetIsDisplayed())
		{
			child->PopulateRenderGraph(interfaceRenderGraph);
		}
	}
}

void WChildrenOwner::SetChildren(const WidgetSet& widgets)
{
	SetState([this, widgets]() {
		for (auto const& child : children_)
		{
			if (child)
			{
				child->SetParent(nullptr);
			}
		}

		children_ = widgets;

		for (auto const& child : children_)
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
	return std::ranges::count_if(children_,
		[](WidgetPtr const& child) { return child != nullptr; }
	);
}

UChildrenOwnerTree::UChildrenOwnerTree(UPtr<WChildrenOwner> const& widget, std::span<UWidgetTree* const> children)
	: widget_{ widget }
	, children_{ children | std::ranges::to<std::vector>() }
{
}

UChildrenOwnerTree::UChildrenOwnerTree(UPtr<WChildrenOwner> const& widget, std::initializer_list<UWidgetTree*> children)
	: widget_{ widget }
	, children_{ children | std::ranges::to<std::vector>() }
{
}

UChildrenOwnerTree::~UChildrenOwnerTree()
{
	for (auto const* widgetTree : children_)
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
	for (auto const* child : children_)
	{
		if (child)
		{
			child->Link();
		}
	}

	if (widget_)
	{
		auto const widgets{ children_
			| std::views::filter([](UWidgetTree const* child) { return child != nullptr; })
			| std::views::transform([](UWidgetTree const* child) { return child->Widget(); })
			| std::ranges::to<USet>()
		};

		widget_->SetChildren(widgets);
	}
}

#include "generated/ChildrenOwner.generated.inl"
