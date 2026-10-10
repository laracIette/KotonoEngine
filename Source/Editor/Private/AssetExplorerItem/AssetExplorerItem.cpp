#include "AssetExplorerItem.h"

#include "AssetExplorer/AssetExplorer.h"
#include <Interface/Interface.h>
#include <core_widgets.h>

WAssetExplorerItem::WAssetExplorerItem(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OpenedCallback const& onOpened)
    : assetExplorer_{ assetExplorer }
    , path_{ path }
    , onOpened_{ onOpened }
    , isSelected_{ false }
    , lastClickedTime_{ 0.0f }
    , openThreshold_{ 0.2f }
{
}

WidgetPtr WAssetExplorerItem::Build()
{
    return (
        UCreate<WBox>{ "Item Box" }()
        | Apply(&WBox::SetSize, glm::vec2{ 128.0f })
        | (
            UCreate<WStack>{ "Item Stack" }()
            | (
                UCreate<WButton>{ "Item Button" }() 
                | Apply(&WButton::SetOnClicked, [this]() {
                    if (isSelected_ 
                     && GetInterface()->GetNow() - lastClickedTime_ < openThreshold_)
                    {
                        if (onOpened_)
                        {
                            onOpened_(path_);
                        }
                    }
                    else
                    {
                        lastClickedTime_ = GetInterface()->GetNow();
                        Select();
                    }
                })
            )
            | (
                UCreate<WAlign>{ "Item Center" }()
                | Apply(&WAlign::SetAlignment, UAlignment::Center())
                | (
                    UCreate<WText>{ "Item Text" }() 
                    | Apply(&WText::SetText, path_.Name())
                )
            )
            | (
                UCreate<WColor>{}()
                | Apply(&WColor::SetIsVisible, [this]() { return isSelected_; })
                | Apply(&WColor::SetColor, Colors::Cyan.WithAlpha(0.1f))
            )
        )
    );
}

void WAssetExplorerItem::Select()
{
    isSelected_ = true;
}

void WAssetExplorerItem::Deselect()
{
    isSelected_ = false;
}

#include "AssetExplorerItem.generated.inl"
