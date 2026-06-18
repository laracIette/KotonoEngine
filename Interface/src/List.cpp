#include "List.h"
#include "widgets.h"
#include "ListBody.h"

WidgetPtr WList::Build()
{
	body_ = UCreate<WListBody>{}();

	UPtr scrollable{ UCreate<WScrollable>{}() };
	scrollable->SetAxis(EAxis::Vertical);
	scrollable->SetChild(body_);

	return scrollable;
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
