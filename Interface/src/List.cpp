#include "List.h"

#include "ListBody.h"
#include "widgets.h"

WidgetPtr WList::Build()
{
	return (
		UCreate<WScrollable>{ "List Scrollable" }()
		| Apply(&WScrollable::SetAxis, EAxis::Vertical)
		| (
			body_ = UCreate<WListBody>{ "List Body" }()
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

void WList::SetSpacing(f32 spacing)
{
	body_->SetSpacing(spacing);
}

void WList::SetChildren(WidgetSet const& children)
{
	body_->SetChildren(children);
}

void WList::AddChild(WidgetPtr const& child)
{
	body_->AddChild(child);
}

#include "generated/List.generated.inl"
