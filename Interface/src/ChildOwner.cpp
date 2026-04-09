#include "ChildOwner.h"

void WChildOwner::CacheBuild()
{
	Base::CacheBuild();
	if (child_)
	{
		child_->CacheBuild();
	}
}

void WChildOwner::Cleanup()
{
	if (child_)
	{
		child_->Cleanup();
		child_->Delete();
	}

	Base::Cleanup();
}

EFlex WChildOwner::GetFlex() const
{
	if (child_)
	{
		return child_->GetFlex();
	}
	return EFlex::None;
}

glm::vec2 WChildOwner::GetDesiredSize(glm::vec2 bounds) const
{
	if (child_)
	{
		return child_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

WidgetVector WChildOwner::GetWidgetTree()
{
	WidgetVector result{ Ptr() };

	if (child_)
	{
		const auto sub{ child_->GetWidgetTree() };
		result.insert(result.end(), sub.begin(), sub.end());
	}

	return result;
}

const WidgetPtr& WChildOwner::GetChild() const
{
	return child_;
}

void WChildOwner::SetChild(const WidgetPtr& widget)
{
	if (widget == child_)
	{
		return;
	}
	if (child_)
	{
		child_->SetParent({});
	}
	child_ = widget;
	if (child_)
	{
		child_->SetParent(Ptr());
	}
}

#include "generated/ChildOwner.generated.inl"
