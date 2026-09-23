#include "HorizontalWrapList.h"

#include "HorizontalWrapListBody.h"
#include "widgets.h"

WidgetPtr WHorizontalWrapList::Build()
{
	return (
		scrollable_ = UCreate<WScrollable>{ "Horizontal Wrap List Scrollable" }()
		| Apply(&WScrollable::SetAxis, EAxis::Vertical)
		| (
			body_ = UCreate<WHorizontalWrapListBody>{ "Horizontal Wrap List Body" }()
		)
	);
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

void WHorizontalWrapList::AddChild(WidgetPtr const & child)
{
	body_->AddChild(child);
}

#include "generated/HorizontalWrapList.generated.inl"
