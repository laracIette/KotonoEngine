#include "CoreWidgets/List/List.h"

#include "ListBody/ListBody.h"
#include "core_widgets.h"

WList::WList()
{
	scrollable_ = UCreate<WScrollable>{ "List Scrollable" }();
	body_ = UCreate<WListBody>{ "List Body" }();
}

WidgetPtr WList::Build()
{
	return (
		UCreate<WCrop>{}()
		| (
			scrollable_
			| Apply(&WScrollable::SetAxis, EAxis::Vertical)
			| (
				body_
			)
		)
	);
}

auto WList::GetSpacing() const -> f32
{
	return body_->GetSpacing();
}

auto WList::GetChildren() const -> WidgetSet const&
{
	return body_->GetChildren();
}

void WList::SetSpacing(f32 spacing) const
{
	body_->SetSpacing(spacing);
}

void WList::SetChildren(WidgetSet const& children) const
{
	body_->SetChildren(children);
	scrollable_->SetOffset({ 0.0f, 0.0f });
}

void WList::AddChild(WidgetPtr const& child) const
{
	body_->AddChild(child);
}

void WList::ReplaceChild(WidgetPtr const& oldWidget, WidgetPtr const& newWidget) const
{
	body_->ReplaceChild(oldWidget, newWidget);
}

#include "generated/List.generated.inl"
