#include "ChildOwner.h"

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

glm::vec2 WChildOwner::GetDesiredSize(const glm::vec2& bounds) const
{
	if (child_)
	{
		return child_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

WidgetVector WChildOwner::WidgetTree() const
{
	WidgetVector result{ Base::WidgetTree() };

	if (child_)
	{
		const auto sub{ child_->WidgetTree() };
		result.insert(result.end(), sub.begin(), sub.end());
	}

	return result;
}

void WChildOwner::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	if (child_ && child_->GetIsDisplayed())
	{
		child_->PopulateRenderGraph(interfaceRenderGraph);
	}
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
			child_->SetParent({});
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

UChildOwnerTree::UChildOwnerTree(const UPtr<WChildOwner>& widget, UWidgetTree* child)
	: widget_(widget)
	, child_(child)
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

