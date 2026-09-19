#include "AssetExplorerItem.h"

#include "AssetExplorer.h"
#include <kotono_core/Interface.h>
#include <kotono_interface/widgets.h>

WAssetExplorerItem::WAssetExplorerItem(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OnClickedFunc const& onDoubleClicked)
    : assetExplorer_{ assetExplorer }
    , path_{ path }
    , onDoubleClicked_{ onDoubleClicked }
    , isSelected_{ false }
    , lastClickedTime_{ 0.0f }
    , doubleClickTreshold_{ 0.2f }
{
}

WidgetPtr WAssetExplorerItem::Build()
{
    const auto widgetTree{ UChildOwnerTree{ UCreate<WBox>{ "Item Box" }()
        | Apply(&WBox::SetSize, glm::vec2{ 128.0f }),

        new UChildrenOwnerTree{ UCreate<WStack>{ "Item Stack" }(), {

            new UWidgetTreeLeaf{ UCreate<WButton>{ "Item Button" }() 
                | Apply(&WButton::SetIsSelectable, false)
                | Apply(&WButton::SetOnClicked, [this]() {
                    if (isSelected_ && GetInterface()->GetNow() - lastClickedTime_ < doubleClickTreshold_)
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
                })
            },

            new UChildOwnerTree{ UCreate<WCenter>{ "Item Center" }()
                | Apply(&WCenter::SetAxis, EAxis::All),

                new UWidgetTreeLeaf{ UCreate<WText>{ "Item Text" }() 
                    | Apply(&WText::SetText, path_.Name())
                    | Apply(&WText::SetFontSize, glm::vec2{ 16.0f, 20.0f })
                }

            },

        } }

    } };

    widgetTree.Link();

    return widgetTree.Widget();
}

void WAssetExplorerItem::Select()
{
    isSelected_ = true;
    lastClickedTime_ = GetInterface()->GetNow();
}

void WAssetExplorerItem::Deselect()
{
    isSelected_ = false;
}

#include "generated/AssetExplorerItem.generated.inl"
