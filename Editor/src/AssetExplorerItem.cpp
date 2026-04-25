#include "AssetExplorerItem.h"
#include <kotono_interface/widgets.h>
#include <kotono_core/TimeManager.h>

WAssetExplorerItem::WAssetExplorerItem(const UPath& path, const OnClickedFunc& onDoubleClicked)
    : path_(path)
    , onDoubleClicked_(onDoubleClicked)
    , isSelected_(false)
    , lastClickedTime_(0.0f)
    , doubleClickTreshold_(0.2f)
{
}

WidgetPtr WAssetExplorerItem::Build()
{
    UPtr bg{ Create<WColor>{}() };
    bg->SetColor(isSelected_ 
        ? Colors::White.WithValue(0.2f) 
        : Colors::White.WithValue(0.1f)
    );

    UPtr text{ Create<WText>{}() };
    text->SetText(path_.Name());
    text->SetFontSize({ 16.0f, 20.0f });
    text->SetSpacing(-4.0f);
    text->SetShouldWrap(true);

    UPtr textCenter{ Create<WCenter>{}() };
    textCenter->SetChild(text);
    textCenter->SetAxis(EAxis::All);

    UPtr button{ Create<WButton>{}() };
    button->SetOnClicked([this]() { 
        if (isSelected_ && TimeManager.Now() - lastClickedTime_ < doubleClickTreshold_) {
            if (onDoubleClicked_) {
                onDoubleClicked_(path_);
            }
        }
        else {
            SetState([this]() {
                lastClickedTime_ = TimeManager.Now();
                isSelected_ = true;
            });
        }
    });
    button->SetOnPressOut([this]() { SetState([this]() { isSelected_ = false; }); });


    UPtr stack{ Create<WStack>{}() };
    stack->SetChildren({ bg, textCenter, button });

    UPtr box{ Create<WBox>{}() };
    box->SetChild(stack);
    box->SetSize({ 128.0f, 128.0f });

    return box;
}

#include "generated/AssetExplorerItem.generated.inl"
