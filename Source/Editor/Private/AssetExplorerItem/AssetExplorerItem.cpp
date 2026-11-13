#include "AssetExplorerItem.h"

#include "AssetExplorer/AssetExplorer.h"
#include <Interface/Interface.h>
#include <core_widgets.h>

WAssetExplorerItem::WAssetExplorerItem(UPath path, OpenedCallback onOpened)
    : path_{ std::move(path) }
    , onOpened_{ std::move(onOpened) }
    , lastClickedTime_{ 0.0f }
    , openThreshold_{ 0.3f }
{
}

auto WAssetExplorerItem::Build() -> WidgetPtr
{
    return (
        UCreate<WBox>{ "Item Box" }()
        | Apply(&WBox::SetSize, glm::vec2{ 128.0f })
        | (
            UCreate<WAlign>{ "Item Center" }()
            | Apply(&WAlign::SetAlignment, UAlignment::Center())
            | (
                UCreate<WText>{ "Item Text" }()
                | Apply(&WText::SetText, path_.Name())
            )
        )
    );
}

void WAssetExplorerItem::Open() const
{
    if (onOpened_)
    {
        onOpened_(path_);
    }
}

#include "AssetExplorerItem.generated.inl"
