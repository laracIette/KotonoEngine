#include "AssetExplorer.h"
#include <kotono_interface/widgets.h>

WidgetPtr WAssetExplorer::Build()
{
	return new WStack({
		.children = {
			new WColor({ Colors::White.WithAlpha(0.1f) }),
			new WPadding({
				.padding = WPadding::Padding::All(8.0f),
				.child = new WHorizontalWrapList({
					.itemSpacing = 10.0f,
					.rowSpacing = 10.0f,
					.children = []() {
						return WidgetVector{
							new WBox({
								.size = { 128.0f, 128.0f },
								.child = new WImage({ "${ENGINE_DIRECTORY}/Graphics/assets/textures/default_texture.jpg" }),
							}),
							new WBox({
								.size = { 64.0f, 64.0f },
								.child = new WColor({ Colors::Green }),
							}),
							new WBox({
								.size = { 32.0f, 32.0f },
								.child = new WColor({ Colors::Blue }),
							}),
							new WBox({
								.size = { 16.0f, 16.0f },
								.child = new WColor({ Colors::Red }),
							}),
						};
					},
				}),
			}),
		},
	});
}
