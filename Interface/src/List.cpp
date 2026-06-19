#include "List.h"
#include "widgets.h"
#include "ListBody.h"

WidgetPtr WList::Build()
{
	const auto widgetTree{ UChildOwnerTree{ UCreate<WScrollable>{ "List Scrollable" }(EAxis::Vertical),
		new UWidgetTreeLeaf{ body_ = UCreate<WListBody>{ "List Body" }() }
	} };
	widgetTree.Link();

	return widgetTree.Widget();
}

float WList::GetSpacing() const
{
	return body_->GetSpacing();
}

const WidgetPool& WList::GetChildren() const
{
	return body_->GetChildren();
}

void WList::SetSpacing(const float spacing)
{
	body_->SetSpacing(spacing);
}

void WList::SetChildren(const WidgetPool& children)
{
	body_->SetChildren(children);
}

#include "generated/List.generated.inl"
