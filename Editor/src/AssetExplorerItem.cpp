#include "AssetExplorerItem.h"
#include <kotono_interface/widgets.h>

WAssetExplorerItem::WAssetExplorerItem(const UPath& path, const OnClickedFunc& onClicked)
    : path_(path)
    , onClicked_(onClicked)
{
}

WidgetPtr WAssetExplorerItem::Build()
{
    return new WBox({
        .size = { 128.0f, 128.0f },
        .child = new WStack({
            .children = {
                new WColor({ Colors::White.WithValue(0.2f) }),
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
                    .onClicked = [this]() { onClicked_(path_); },
                }),
            },
        }),
    });
}
