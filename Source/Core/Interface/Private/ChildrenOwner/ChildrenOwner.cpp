#include "ChildrenOwner/ChildrenOwner.h"

#include <algorithm>
#include <math_utils/math_utils.h>		 
#include <ranges>

WChildrenOwner::WChildrenOwner()
	: children_{}
{
}

WChildrenOwner::~WChildrenOwner()
{
	for (auto const& child : WidgetSet{ children_ })
	{		
		Check(Warning, child, "child is null!");
		
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
		Check(Warning, child, "child is null!");
		
		if (child)
		{
			child->Remove();
		}
	}
}

void WChildrenOwner::Disown(WidgetPtr const& widget)
{
	if (!widget)
	{
		return;
	}

	auto const it{ children_.Find(widget) };

	if (children_.IsValidIterator(it))
	{
		SetState([this, widget, it]() {
			children_.Remove(it);
			widget->SetParent(nullptr);
		});
	}
}

void WChildrenOwner::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint) const
{
	for (auto const& child : children_)
	{
		if (!child)
		{
			Log(Warning, "child is null!");
			continue;
		}
		
		if (child->GetIsDisplayed() && GetIsContentVisible() && (Tint != Colors::Transparent))
		{
			child->PopulateRenderGraph(interfaceRenderGraph, tint * Tint);
		}
	}
}

void WChildrenOwner::PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const
{
	Base::PopulateFocusTree(widgets, cursorPosition);

	for (auto const& child : children_)
	{		
		if (!child)
		{
			Log(Warning, "child is null!");
			continue;
		}
		
		if (child->GetIsDisplayed())
		{
			child->PopulateFocusTree(widgets, cursorPosition);
		}
	}
}

void WChildrenOwner::Refresh()
{
	if (GetShouldRefresh())
	{
		return Base::Refresh();
	}

	for (auto const& child : children_)
	{
		Check(Warning, child, "child is null!");
		
		if (child)
		{
			child->Refresh();
		}
	}
}

void WChildrenOwner::CacheBuild()
{
	for (auto const& child : children_)
	{
		Check(Warning, child, "child is null!");
		
		if (child)
		{
			child->CacheBuild();
		}
	}
}

void WChildrenOwner::SetChildren(WidgetSet const& widgets)
{
	SetState([this, widgets]() {
		for (auto const& child : children_)
		{
			Check(Warning, child, "child is null!");
		
			if (child)
			{
				child->SetParent(nullptr);
			}
		}

		for (auto const& child : widgets)
		{
			Check(Warning, child, "child is null!");
		
			if (child)
			{
				child->SetParent(Ptr());
			}
		}
		
		children_ = widgets;
	});
}

void WChildrenOwner::AddChild(WidgetPtr const& widget)
{
	if (!widget)
	{
		return;
	}

	SetState([this, widget]() {
		widget->SetParent(Ptr());
		children_.Add(widget);
	});
}

void WChildrenOwner::RemoveChild(WidgetPtr const& widget)
{
	if (!widget)
	{
		return;
	}

	SetState([this, widget]() {
		widget->SetParent(nullptr);
		children_.Remove(widget);
	});
}

void WChildrenOwner::ReplaceChild(WidgetPtr const& oldWidget, WidgetPtr const& newWidget)
{
	SetState([this, oldWidget, newWidget]() {
		if (oldWidget)
		{
			oldWidget->SetParent(nullptr);
		}

		children_.Replace(oldWidget, newWidget);

		if (newWidget)
		{
			newWidget->SetParent(Ptr());
		}
	});
}

size WChildrenOwner::GetValidChildrenCount() const
{
	return std::ranges::count_if(children_,
		[](WidgetPtr const& child) { return child != nullptr; }
	);
}

#include "ChildrenOwner.generated.inl"
