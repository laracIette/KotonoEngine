#include "ChildrenOwner.h"

#include <algorithm>
#include <kotono_math/math_utils.h>		 
#include <ranges>

WChildrenOwner::~WChildrenOwner()
{
	for (auto const& child : children_)
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

	for (auto const& child : children_)
	{
		if (child)
		{
			child->Remove();
		}
	}
}

void WChildrenOwner::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	for (auto const& child : children_)
	{
		if (child && child->GetIsDisplayed())
		{
			child->PopulateRenderGraph(interfaceRenderGraph);
		}
	}
}

void WChildrenOwner::PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const
{
	Base::PopulateFocusTree(widgets, cursorPosition);

	for (auto const& child : children_)
	{
		if (child && child->GetIsDisplayed())
		{
			child->PopulateFocusTree(widgets, cursorPosition);
		}
	}
}

b8 WChildrenOwner::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position)
{
	for (auto const& child : children_ | std::views::reverse)
	{
		if (!child || !child->GetIsDisplayed())
		{
			continue;
		}

		if (!is_point_in_rect(position, child->GetPosition(), child->GetSize()))
		{
			continue;
		}

		if (child->OnMouseButton(button, inputState, position) == INPUT_HANDLED)
		{
			return INPUT_HANDLED;
		}
	}

	return INPUT_UNHANDLED;
}

b8 WChildrenOwner::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position)
{
	for (auto const& child : children_ | std::views::reverse)
	{
		if (!child || !child->GetIsDisplayed())
		{
			continue;
		}

		if (!is_point_in_rect(position, child->GetPosition(), child->GetSize()))
		{
			continue;
		}

		if (child->OnMouseMove(delta, position) == INPUT_HANDLED)
		{
			return INPUT_HANDLED;
		}
	}

	return INPUT_UNHANDLED;
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
