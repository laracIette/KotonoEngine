#include "ChildOwner.h"

#include <kotono_math/math_utils.h>

WChildOwner::~WChildOwner()
{
	if (child_)
	{
		child_->Delete();
	}
}

void WChildOwner::Remove()
{
	Base::Remove();

	if (child_)
	{
		child_->Remove();
	}
}

glm::vec2 WChildOwner::GetContentSize(glm::vec2 const& bounds) const
{
	if (child_)
	{
		return child_->GetContentSize(bounds);
	}
	return { 0.0f, 0.0f };
}

glm::vec2 WChildOwner::GetDesiredSize(const glm::vec2& bounds) const
{
	if (child_)
	{
		return child_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

void WChildOwner::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	if (child_ && child_->GetIsDisplayed())
	{
		child_->PopulateRenderGraph(interfaceRenderGraph);
	}
}

void WChildOwner::PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const
{
	Base::PopulateFocusTree(widgets, cursorPosition);

	if (child_ && child_->GetIsDisplayed())
	{
		child_->PopulateFocusTree(widgets, cursorPosition);
	}
}

b8 WChildOwner::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position)
{
	if (!child_ || !child_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (is_point_in_rect(position, child_->GetPosition(), child_->GetSize()))
	{
		return child_->OnMouseButton(button, inputState, position);
	}

	return INPUT_UNHANDLED;
}

b8 WChildOwner::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position)
{
	if (!child_ || !child_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (is_point_in_rect(position, child_->GetPosition(), child_->GetSize()))
	{
		return child_->OnMouseMove(delta, position);
	}

	return INPUT_UNHANDLED;
}

b8 WChildOwner::OnMouseScroll(glm::vec2 const& delta)
{
	if (!child_ || !child_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (child_->GetIsFocused())
	{
		return child_->OnMouseScroll(delta);
	}

	return INPUT_UNHANDLED;
}

b8 WChildOwner::OnKeyboardKey(EKey key, EInputState inputState)
{
	if (!child_ || !child_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (child_->GetIsFocused())
	{
		return child_->OnKeyboardKey(key, inputState);
	}

	return INPUT_UNHANDLED;
}

void WChildOwner::Refresh()
{
	if (GetShouldRefresh())
	{
		return Base::Refresh();
	}

	if (child_)
	{
		child_->Refresh();
	}
}

void WChildOwner::SetChild(WidgetPtr const& widget)
{
	if (widget == child_)
	{
		return;
	}

	SetState([this, widget]() {
		if (child_)
		{
			child_->SetParent(nullptr);
		}

		child_ = widget;

		if (child_)
		{
			child_->SetParent(Ptr());
		}
	});
}

void WChildOwner::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (child_)
	{
		child_->Display(displaySettings);
	}
}

#include "generated/ChildOwner.generated.inl"

