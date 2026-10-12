#include "ChildOwner/ChildOwner.h"

#include <math_utils/math_utils.h>

WChildOwner::WChildOwner()
	: child_{ nullptr }
{
}

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

void WChildOwner::Disown(WidgetPtr const& widget)
{
	if (!widget)
	{
		return;
	}

	if (widget != child_)
	{
		return;
	}

	SetState([this, widget]() {
		child_ = nullptr;
		widget->SetParent(nullptr);
	});
}

auto WChildOwner::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
{
	if (child_)
	{
		return child_->GetContentSize(bounds);
	}
	return { 0.0f, 0.0f };
}

auto WChildOwner::GetDesiredSize(const glm::vec2& bounds) const -> glm::vec2
{
	if (child_)
	{
		return child_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

void WChildOwner::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	if (child_ && child_->GetIsDisplayed() && GetIsContentVisible())
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

void WChildOwner::CacheBuild()
{
	if (child_)
	{
		child_->CacheBuild();
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

#include "ChildOwner.generated.inl"

