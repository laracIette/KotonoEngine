#include "ChildrenOwner.h"
#include <algorithm>
#include <kotono_common/bitwise_utils.h>

void WChildrenOwner::CacheBuild()
{
	Base::CacheBuild();
	for (auto& child : children_)
	{
		if (child)
		{
			child->CacheBuild();
		}
	}
}

void WChildrenOwner::Cleanup()
{
	for (auto& child : children_)
	{
		if (child)
		{
			child->Cleanup();
			child->Delete();
		}
	}
	children_.Clear();

	Base::Cleanup();
}

EFlex WChildrenOwner::GetFlex() const
{
	EFlex result{ EFlex::None };

	for (auto& child : children_)
	{
		if (child)
		{
			result = bitwise_or(result, child->GetFlex());
		}
	}

	return result;
}

WidgetVector WChildrenOwner::GetWidgetTree()
{
	WidgetVector result{ Ptr() };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto sub{ child->GetWidgetTree() };
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
}

void WChildrenOwner::AddChild(const WidgetPtr& widget)
{
	if (widget)
	{
		widget->SetParent(Ptr());
		children_.Add(widget);
	}
}

void WChildrenOwner::RemoveChild(const WidgetPtr& widget)
{
	if (widget)
	{
		widget->SetParent({});
		children_.Remove(widget);
	}
}

size WChildrenOwner::GetValidChildrenCount() const
{
	return std::count_if(children_.begin(), children_.end(),
		[](const WidgetPtr& child) { return child != WidgetPtr{}; }
	);
}

#include "generated/ChildrenOwner.generated.inl"
