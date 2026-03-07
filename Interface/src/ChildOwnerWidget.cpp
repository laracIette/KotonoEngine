#include "ChildOwnerWidget.h"

WChildOwnerWidget::WChildOwnerWidget(WidgetPtr child) :
	child_(child)
{
	if (child_)
	{
		child_->SetParent(this);
	}
}

void WChildOwnerWidget::CacheBuild()
{
	WWidget::CacheBuild();
	if (child_)
	{
		child_->CacheBuild();
	}
}

void WChildOwnerWidget::Cleanup()
{
	if (child_)
	{
		child_->Cleanup();
	}
	delete child_;
	child_ = nullptr;

	WWidget::Cleanup();
}

EFlex WChildOwnerWidget::GetFlex() const
{
	if (child_)
	{
		return child_->GetFlex();
	}
	return EFlex::None;
}

glm::vec2 WChildOwnerWidget::GetDesiredSize(glm::vec2 bounds) const
{
	if (child_)
	{
		return child_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

WidgetVector WChildOwnerWidget::GetWidgetTree()
{
	WidgetVector result{ this };

	if (child_)
	{
		const auto sub{ child_->GetWidgetTree() };
		result.insert(result.end(), sub.begin(), sub.end());
	}

	return result;
}
