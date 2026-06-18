#include "HorizontalWrapList.h"
#include "HorizontalWrapListBody.h"
#include "widgets.h"

WidgetPtr WHorizontalWrapList::Build()
{
	body_ = UCreate<WHorizontalWrapListBody>{}();

	UPtr scrollable{ UCreate<WScrollable>{}() };
	scrollable->SetAxis(EAxis::Vertical);
	scrollable->SetChild(body_);

	return scrollable;
}

float WHorizontalWrapList::GetItemSpacing() const
{
	return body_->GetItemSpacing();
}

float WHorizontalWrapList::GetRowSpacing() const
{
	return body_->GetRowSpacing();
}

const WidgetPool& WHorizontalWrapList::GetChildren() const
{
	return body_->GetChildren();
}

void WHorizontalWrapList::SetItemSpacing(const float itemSpacing)
{
	body_->SetItemSpacing(itemSpacing);
}

void WHorizontalWrapList::SetRowSpacing(const float rowSpacing)
{
	body_->SetRowSpacing(rowSpacing);
}

void WHorizontalWrapList::SetChildren(const WidgetPool& children)
{
	body_->SetChildren(children);
}

#include "generated/HorizontalWrapList.generated.inl"
