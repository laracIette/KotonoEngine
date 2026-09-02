#include "HorizontalWrapList.h"

#include "HorizontalWrapListBody.h"
#include "widgets.h"

WidgetPtr WHorizontalWrapList::Build()
{
	const auto widgetTree{ UChildOwnerTree{ scrollable_ = UCreate<WScrollable>{ "Horizontal Wrap List Scrollable" }(EAxis::Vertical),
		new UWidgetTreeLeaf{ body_ = UCreate<WHorizontalWrapListBody>{ "Horizontal Wrap List Body" }() }
	} };
	widgetTree.Link();

	return widgetTree.Widget();
}

f32 WHorizontalWrapList::GetItemSpacing() const
{
	return body_->GetItemSpacing();
}

f32 WHorizontalWrapList::GetRowSpacing() const
{
	return body_->GetRowSpacing();
}

WidgetSet const& WHorizontalWrapList::GetChildren() const
{
	return body_->GetChildren();
}

void WHorizontalWrapList::SetItemSpacing(f32 itemSpacing)
{
	body_->SetItemSpacing(itemSpacing);
}

void WHorizontalWrapList::SetRowSpacing(f32 rowSpacing)
{
	body_->SetRowSpacing(rowSpacing);
}

void WHorizontalWrapList::SetChildren(WidgetSet const& children)
{
	body_->SetChildren(children);
	scrollable_->SetOffset(glm::vec2{ 0.0f });
}

#include "generated/HorizontalWrapList.generated.inl"
