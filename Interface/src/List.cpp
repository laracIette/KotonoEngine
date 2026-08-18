#include "List.h"

#include "ListBody.h"
#include "widgets.h"

WidgetPtr WList::Build()
{
	const auto widgetTree{ UChildOwnerTree{ UCreate<WScrollable>{ "List Scrollable" }(EAxis::Vertical),
		new UWidgetTreeLeaf{ body_ = UCreate<WListBody>{ "List Body" }() }
	} };
	widgetTree.Link();

	return widgetTree.Widget();
}

f32 WList::GetSpacing() const
{
	return body_->GetSpacing();
}

WidgetSet const& WList::GetChildren() const
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

#include "generated/List.generated.inl"
