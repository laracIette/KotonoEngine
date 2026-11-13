#include "CoreWidgets/HorizontalWrapList/HorizontalWrapList.h"

#include "HorizontalWrapListBody/HorizontalWrapListBody.h"
#include "core_widgets.h"

WHorizontalWrapList::WHorizontalWrapList()
{
	scrollable_ = UCreate<WScrollable>{ "Horizontal Wrap List Scrollable" }();
	body_ = UCreate<WHorizontalWrapListBody>{ "Horizontal Wrap List Body" }();
}

WHorizontalWrapList::~WHorizontalWrapList()
{
	Delete(scrollable_);
	Delete(body_);
}

auto WHorizontalWrapList::Build() -> WidgetPtr
{
	return (
		UCreate<WCrop>{}()
		| (
			scrollable_
			| Apply(&WScrollable::SetAxis, EAxis::Vertical)
			| (
				body_
			)
		)
	);
}

auto WHorizontalWrapList::GetItemSpacing() const -> f32
{
	return body_->GetItemSpacing();
}

auto WHorizontalWrapList::GetRowSpacing() const -> f32
{
	return body_->GetRowSpacing();
}

auto WHorizontalWrapList::GetChildren() const -> WidgetSet const&
{
	return body_->GetChildren();
}

void WHorizontalWrapList::SetChildren(WidgetSet const& children)
{
    body_->SetChildren(children);
    scrollable_->SetOffset({ 0.0f, 0.0f });
}

void WHorizontalWrapList::AddChild(WidgetPtr const& child)
{
    body_->AddChild(child);
}

void WHorizontalWrapList::SetItemSpacing(f32 itemSpacing) const
{
	body_->SetItemSpacing(itemSpacing);
}

void WHorizontalWrapList::SetRowSpacing(f32 rowSpacing) const
{
	body_->SetRowSpacing(rowSpacing);
}

#include "HorizontalWrapList.generated.inl"
