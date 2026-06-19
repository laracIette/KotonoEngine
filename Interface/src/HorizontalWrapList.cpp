#include "HorizontalWrapList.h"
#include "HorizontalWrapListBody.h"
#include "widgets.h"

WidgetPtr WHorizontalWrapList::Build()
{
	const auto widgetTree{ UChildOwnerTree{ UCreate<WScrollable>{ "Horizontal Wrap List Scrollable" }(EAxis::Vertical),
		new UWidgetTreeLeaf{ body_ = UCreate<WHorizontalWrapListBody>{ "Horizontal Wrap List Body" }() }
	} };
	widgetTree.Link();

	return widgetTree.Widget();
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
