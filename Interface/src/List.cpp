#include "List.h"
#include "widgets.h"
#include "ListBody.h"

WidgetPtr WList::Build()
{
	listBody_ = Create<WListBody>{}();
	listBody_->SetSpacing(spacing_);

	UPtr scrollable{ Create<WScrollable>{}() };
	scrollable->SetAxis(EAxis::Vertical);
	scrollable->SetChild(listBody_);

	return scrollable;
}

float WList::GetSpacing() const
{
	return spacing_;
}

const WidgetPool& WList::GetChildren() const
{
	return children_;
}

void WList::SetSpacing(const float spacing)
{
	spacing_ = spacing;
	listBody_->SetSpacing(spacing);
}

void WList::SetChildren(const WidgetPool& children)
{
	children_ = children;
	listBody_->SetChildren(children);
}

#include "generated/List.generated.inl"
