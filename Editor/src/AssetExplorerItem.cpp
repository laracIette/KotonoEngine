#include "AssetExplorerItem.h"

#include "AssetExplorer.h"
#include <kotono_interface/widgets.h>
#include <kotono_object/Interface.h>

static constexpr UColor NORMAL_COLOR{ Colors::White.WithValue(0.1f).WithAlpha(0.75f) };
static constexpr UColor SELECTED_COLOR{ Colors::Blue.WithAlpha(0.3f) };
static constexpr UColor FOCUSED_COLOR{ Colors::White.WithValue(0.15f).WithAlpha(0.75f) };

WAssetExplorerItem::WAssetExplorerItem(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OnClickedFunc const& onDoubleClicked)
    : assetExplorer_{ assetExplorer }
    , path_{ path }
    , onDoubleClicked_{ onDoubleClicked }
    , isSelected_{ false }
    , lastClickedTime_{ 0.0f }
    , doubleClickTreshold_{ 0.2f }
    , background_{}
{
}

WidgetPtr WAssetExplorerItem::Build()
{
    UPtr text{ UCreate<WText>{ "Item Text" }() };
    text->SetText(path_.Name());
    text->SetFontSize({ 16.0f, 20.0f });
    text->SetSpacing(-4.0f);
    text->SetShouldWrap(true);

    UPtr button{ UCreate<WButton>{ "Item Button" }() };
    button->SetOnClicked([this]() {
        if (isSelected_ && GetInterface()->GetTimeContext().total - lastClickedTime_ < doubleClickTreshold_)
        {
            if (onDoubleClicked_)
            {
                onDoubleClicked_(path_);
            }
        }
        else
        {
            Select();
        }
    });

    button->SetOnPressOut([this]() { Deselect(); });

    const auto widgetTree{ UChildOwnerTree{ UCreate<WBox>{ "Item Box" }(glm::vec2{ 128.0f }),
        new UChildrenOwnerTree{ UCreate<WStack>{ "Item Stack" }(), {
            new UWidgetTreeLeaf{ background_ = UCreate<WColor>{ "Item Background" }(NORMAL_COLOR) },
            new UChildOwnerTree{ UCreate<WCenter>{ "Item Center" }(EAxis::All),
                new UWidgetTreeLeaf{ text }
            },
            new UWidgetTreeLeaf{ button },
        } }
    } };
    widgetTree.Link();

    return widgetTree.Widget();
}

void WAssetExplorerItem::Select()
{
    isSelected_ = true;
    lastClickedTime_ = GetInterface()->GetTimeContext().total;
    background_->SetColor(SELECTED_COLOR);
    if (assetExplorer_)
    {
        assetExplorer_->DeselectOthers(Ptr());
    }
}

void WAssetExplorerItem::Deselect()
{
    isSelected_ = false;
    background_->SetColor(NORMAL_COLOR);
}

void WAssetExplorerItem::OnFocused()
{
    Base::OnFocused();

    if (!isSelected_)
    {
        background_->SetColor(FOCUSED_COLOR);
    }
}

void WAssetExplorerItem::OnUnfocused()
{
    Base::OnUnfocused();

    if (!isSelected_)
    {
        background_->SetColor(NORMAL_COLOR);
    }
}

#include "generated/AssetExplorerItem.generated.inl"
