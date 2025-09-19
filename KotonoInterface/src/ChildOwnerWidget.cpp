#include "ChildOwnerWidget.h"

WChildOwnerWidget::WChildOwnerWidget(WWidget* child) :
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

WWidget::WidgetVector WChildOwnerWidget::GetWidgetTree()
{
	WidgetVector result{ this };

	if (child_)
	{
		const auto sub = child_->GetWidgetTree();
		result.insert(result.end(), sub.begin(), sub.end());
	}

	return result;
}
