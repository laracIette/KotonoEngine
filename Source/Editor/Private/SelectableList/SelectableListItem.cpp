#include "SelectableListItem.h"

#include <core_widgets.h>

WSelectableListItem::WSelectableListItem()
{
	socket_ = UCreate<WSocket>{}();
}

WSelectableListItem::~WSelectableListItem()
{
    Delete(socket_);
}

auto WSelectableListItem::Build() -> WidgetPtr
{
	return ( 
		UCreate<WWrap>{}()
		| Apply(&WWrap::SetAxis, EAxis::Vertical)
		| (
			UCreate<WStack>{}()
			| (
			    UCreate<WButton>{}()
			    | Apply(&WButton::SetOnClicked, [this](EModifier modifier) {
                    if (onClicked_)
                    {
                        onClicked_(modifier);
			        }
			    })
			)
			| (
			    UCreate<WColor>{}()
			    | Apply(&WColor::SetColor, [this]() { return (isSelected_ && isSelected_()) ? Colors::Blue.WithAlpha(0.2f) : Colors::Transparent; })
			)
			| (
				socket_
			)
		)
	);
}

void WSelectableListItem::SetChild(WidgetPtr const& widget) const
{
    Check(Throw, socket_, "socket_ is null");
    Delete(socket_->GetChild());
	socket_->SetChild(widget);
}

#include "SelectableListItem.generated.inl"
