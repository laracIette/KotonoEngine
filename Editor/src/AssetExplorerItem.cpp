#include "AssetExplorerItem.h"
#include <kotono_interface/widgets.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <stdexcept>
#endif

WAssetExplorerItem::WAssetExplorerItem(const UPath& path)
    : path_(path)
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
                        .text = path_.ToString(),
                        .fontSize = { 10.0f, 12.0f },
                        .spacing = -4.0f,
                        .shouldWrap = true,
                    }),
                }),
                new WButton({
                    .onClicked = [this]() {
                        const auto string{ std::format("\"{0}\"", path_.ToString())};

#                       ifdef _WIN32
                            ShellExecute(0, 0, string.c_str(), 0, 0, SW_SHOW);
#                       else
                            throw std::exception("Open file not implemented for your OS.");
#                       endif
                    },
                }),
            },
        }),
    });
}
