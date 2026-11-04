#include "SelectableListItem.h"

#include <core_widgets.h>

#include "enum_utils.h"

WSelectableListItem::WSelectableListItem()
{
	button_ = UCreate<WButton>{}();
	socket_ = UCreate<WSocket>{}();
}

auto WSelectableListItem::Build() -> WidgetPtr
{
	return ( 
		UCreate<WWrap>{}()
		| (
			UCreate<WStack>{}()
			| (
				button_
				| Apply(&WButton::SetOnClicked, [this]() {
					button_->SetIsActivated(!button_->GetIsActivated());
					
					if (button_->GetIsActivated() && onSelected_)
					{
						onSelected_();
					}
				})
			)
			| (
				socket_
			)
		)
	);
}

void WSelectableListItem::Activate() const
{
	button_->SetIsActivated(true);
}

void WSelectableListItem::Deactivate() const
{
	button_->SetIsActivated(false);
}

void WSelectableListItem::SetChild(WidgetPtr const& widget) const
{
	socket_->SetChild(widget);
}

#include "SelectableListItem.generated.inl"
