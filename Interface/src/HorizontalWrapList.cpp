#include "HorizontalWrapList.h"
#include "HorizontalWrapListBody.h"
#include "widgets.h"

WidgetPtr WHorizontalWrapList::Build()
{
	UPtr horizontalWrapListBody{ Create<WHorizontalWrapListBody>{}() };
	horizontalWrapListBody->SetItemSpacing(itemSpacing_);
	horizontalWrapListBody->SetRowSpacing(rowSpacing_);
	horizontalWrapListBody->SetChildren(children_);

	UPtr scrollable{ Create<WScrollable>{}() };
	scrollable->SetAxis(EAxis::Vertical);
	scrollable->SetChild(horizontalWrapListBody);

	return scrollable;
}

float WHorizontalWrapList::GetItemSpacing() const
{
	return itemSpacing_;
}

float WHorizontalWrapList::GetRowSpacing() const
{
	return rowSpacing_;
}

void WHorizontalWrapList::SetItemSpacing(const float itemSpacing)
{
	itemSpacing_ = itemSpacing;
}

void WHorizontalWrapList::SetRowSpacing(const float rowSpacing)
{
	rowSpacing_ = rowSpacing;
}

#include "generated/HorizontalWrapList.generated.inl"
