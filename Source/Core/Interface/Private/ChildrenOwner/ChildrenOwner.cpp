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

void WChildrenOwner::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	for (auto const& child : children_)
	{
		Check(Warning, child, "child is null!");
		
		if (!child)
		{
			continue;
		}
		
		if (child->GetIsDisplayed() && GetIsContentVisible())
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
		Check(Warning, child, "child is null!");
		
		if (!child)
		{
			continue;
		}
		
		if (child->GetIsDisplayed())
		{
			child->PopulateFocusTree(widgets, cursorPosition);
		}
	}
}

b8 WChildrenOwner::OnMouseButton(EButton button, EInputState inputState, EModifier modifier)
{
	for (auto const& child : children_ | std::views::reverse)
	{
		Check(Warning, child, "child is null!");
		
		if (!child)
		{
			continue;
		}
		
		if (!child->GetIsDisplayed())
		{
			continue;
		}

		if (!child->GetIsFocused())
		{
			continue;
		}

		if (child->OnMouseButton(button, inputState, modifier) == INPUT_HANDLED)
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
		Check(Warning, child, "child is null!");
		
		if (!child)
		{
			continue;
		}
		
		if (!child->GetIsDisplayed())
		{
			continue;
		}

		if (!child->GetIsPointHovering(position))
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

b8 WChildrenOwner::OnMouseScroll(glm::vec2 const& delta)
{
	for (auto const& child : children_ | std::views::reverse)
	{
		Check(Warning, child, "child is null!");
		
		if (!child)
		{
			continue;
		}
		
		if (!child->GetIsDisplayed())
		{
			continue;
		}

		if (!child->GetIsFocused())
		{
			continue;
		}

		if (child->OnMouseScroll(delta) == INPUT_HANDLED)
		{
			return INPUT_HANDLED;
		}
	}

	return INPUT_UNHANDLED;
}

b8 WChildrenOwner::OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier)
{
	for (auto const& child : children_ | std::views::reverse)
	{
		Check(Warning, child, "child is null!");
		
		if (!child)
		{
			continue;
		}
		
		if (!child->GetIsDisplayed())
		{
			continue;
		}

		if (!child->GetIsFocused())
		{
			continue;
		}

		if (child->OnKeyboardKey(key, inputState, modifier) == INPUT_HANDLED)
		{
			return INPUT_HANDLED;
		}
	}

	return INPUT_UNHANDLED;
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
