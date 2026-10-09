#include "CoreWidgets/Switcher/Switcher.h"

#include "core_widgets.h"

WSwitcher::WSwitcher()
	: activeWidget_{ 0 }
{
	stack_ = UCreate<WStack>{}();
}

WidgetPtr WSwitcher::Build()
{
	return stack_;
}

auto WSwitcher::GetChildren() const -> WidgetSet const&
{
	Check(Abort, stack_, "stack_ is null!");
	return stack_->GetChildren();
}

void WSwitcher::SetChildren(WidgetSet const& children) const
{
	Check(ErrorReturn, stack_, "stack_ is null!");
	stack_->SetChildren(children);
	UpdateChildrenVisibility();
}

void WSwitcher::AddChild(WidgetPtr const& child) const
{
	Check(ErrorReturn, stack_, "stack_ is null!");
	stack_->AddChild(child);
	Check(ErrorReturn, child, "child is null!");
	child->SetIsVisible(GetChildren().LastIndex() == activeWidget_);
}

void WSwitcher::SetActiveWidget(u32 activeWidget)
{
	Check(Warning, activeWidget < GetChildren().size(), "activeWidget should be lower than the children count!");
	activeWidget_ = activeWidget;
	UpdateChildrenVisibility();
}

void WSwitcher::UpdateChildrenVisibility() const
{	
	for (auto const& [index, child] : GetChildren() | std::views::enumerate)
	{
		Check(Throw, child, "child is null!");
		child->SetIsVisible(activeWidget_ == index);
	}
}

#include "Switcher.generated.inl"
