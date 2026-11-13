#include "SelectableListItem.h"

#include "core_widgets.h"
#include "Interface/Interface.h"

static constexpr auto DOUBLE_CLICK_THRESHOLD{ 0.3f };

WSelectableListItem::WSelectableListItem()
    : lastClickTime_{ 0.0f }
    , wrapAxis_{ EAxis::All }
{
    buttonStyleClass_ = KButtonStyle_Class::Get();
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
		| Apply(&WWrap::SetAxis, wrapAxis_)
		| (
			UCreate<WStack>{}()
			| (
			    UCreate<WButton>{}()
			    | Apply(&WButton::SetStyleClass, buttonStyleClass_)
			    | Apply(&WButton::SetOnClicked, [this](EModifier modifier) {
			        if (onClicked_)
                    {
                        onClicked_(modifier);
			        }

			        if (modifier == EModifier::Control)
			        {
			            lastClickTime_ = 0.0f;
			        }
			        else if (GetInterface()->GetNow() - lastClickTime_ < DOUBLE_CLICK_THRESHOLD)
			        {
                        if (onDoubleClicked_)
                        {
                            onDoubleClicked_();
                        }
                    }
                    else
                    {
                        lastClickTime_ = GetInterface()->GetNow();
                    }
			    })
			)
			| (
			    UCreate<WColor>{}()
			    | Apply(&WColor::SetColor, [this]() {
			        return isSelected_.Get() ? Colors::Blue.WithAlpha(0.2f) : Colors::Transparent;
			    })
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
