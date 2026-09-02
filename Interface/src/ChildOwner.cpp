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

glm::vec2 WChildOwner::GetContentSize(glm::vec2 bounds) const
{
	if (child_)
	{
		return child_->GetContentSize(bounds);
	}
	return bounds;
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

void WChildOwner::SetChild(const WidgetPtr& widget)
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
	++displaySettings.layer;

	if (child_)
	{
		child_->Display(displaySettings);
	}
}

UChildOwnerTree::UChildOwnerTree(UPtr<WChildOwner> const& widget, UWidgetTree* child)
	: widget_{ widget }
	, child_{ child }
{
}

UChildOwnerTree::~UChildOwnerTree()
{
	delete child_;
}

WidgetPtr UChildOwnerTree::Widget() const
{
	return widget_;
}

void UChildOwnerTree::Link() const
{
	if (child_)
	{
		child_->Link();
		if (widget_)
		{
			widget_->SetChild(child_->Widget());
		}
	}
}

#include "generated/ChildOwner.generated.inl"

