#include "AssetExplorerItem.h"
#include <kotono_interface/widgets.h>
#include <kotono_object/Interface.h>

WAssetExplorerItem::WAssetExplorerItem(const UPath& path, const OnClickedFunc& onDoubleClicked)
    : path_{ path }
    , onDoubleClicked_{ onDoubleClicked }
    , isSelected_{ false }
    , lastClickedTime_{ 0.0f }
    , doubleClickTreshold_{ 0.2f }
    , background_{}
{
}

WidgetPtr WAssetExplorerItem::Build()
{
    constexpr UColor SELECTED_COLOR{ Colors::White.WithValue(0.2f).WithAlpha(0.75f) };
    constexpr UColor UNSELECTED_COLOR{ Colors::White.WithValue(0.1f).WithAlpha(0.75f) };

    UPtr text{ UCreate<WText>{ "Item Text" }() };
    text->SetText(path_.Name());
    text->SetFontSize({ 16.0f, 20.0f });
    text->SetSpacing(-4.0f);
    text->SetShouldWrap(true);

    UPtr button{ UCreate<WButton>{ "Item Button" }() };
    button->SetOnClicked([this, SELECTED_COLOR]() { 
        if (isSelected_ && GetInterface()->GetTimeContext().total - lastClickedTime_ < doubleClickTreshold_) {
            if (onDoubleClicked_) {
                onDoubleClicked_(path_);
            }
        }
        else {
            lastClickedTime_ = GetInterface()->GetTimeContext().total;
            isSelected_ = true;
            background_->SetColor(SELECTED_COLOR);
        }
    });
    button->SetOnPressOut([this, UNSELECTED_COLOR]() {
        isSelected_ = false;
        background_->SetColor(UNSELECTED_COLOR);
    });

    const auto widgetTree{ UChildOwnerTree{ UCreate<WBox>{ "Item Box" }(glm::vec2{ 128.0f }),
        new UChildrenOwnerTree{ UCreate<WStack>{ "Item Stack" }(), {
            new UWidgetTreeLeaf{ background_ = UCreate<WColor>{ "Item Background" }(UNSELECTED_COLOR) },
            new UChildOwnerTree{ UCreate<WCenter>{ "Item Center" }(EAxis::All), 
                new UWidgetTreeLeaf{ text }
            },
            new UWidgetTreeLeaf{ button },
        } }
    } };
    widgetTree.Link();

    return widgetTree.Widget();
}

#include "generated/AssetExplorerItem.generated.inl"
