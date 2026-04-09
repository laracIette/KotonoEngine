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
    return new WBox({
        .size = { 128.0f, 128.0f },
        .child = new WStack({
            .children = {
                isSelected_ 
                    ? new WColor({ Colors::White.WithValue(0.2f) })
                    : new WColor({ Colors::White.WithValue(0.1f) }),
                new WCenter({
                    .axis = EAxis::All,
                    .child = new WText({
                        .text = path_.Name(),
                        .fontSize = { 16.0f, 20.0f },
                        .spacing = -4.0f,
                        .shouldWrap = true,
                    }),
                }),
                new WButton({
                    .onClicked = [this]() { 
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
                    },
                    .onPressOut = [this]() { SetState([this]() { isSelected_ = false; }); },
                }),
            },
        }),
    });
}

#include "generated/AssetExplorerItem.generated.inl"
