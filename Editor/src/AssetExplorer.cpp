#include "AssetExplorer.h"
#include "AssetExplorerItem.h"
#include <kotono_interface/widgets.h>
#include <kotono_io/File.h>
#include <kotono_io/FileExplorer.h>
#include <ranges>
#include <algorithm>

WidgetPtr WAssetExplorer::Build()
{
	const UFileExplorer fileExplorer("${ENGINE_DIRECTORY}");
	const auto directories{ fileExplorer.GetDirectories() };
	const auto files{ fileExplorer.GetFiles() };

	return new WStack({
		.children = {
			new WColor({ Colors::White.WithAlpha(0.1f) }),
			new WPadding({
				.padding = WPadding::Padding::All(8.0f),
				.child = new WHorizontalWrapList({
					.itemSpacing = 10.0f,
					.rowSpacing = 10.0f,
					.children = [directories, files]() {
						WidgetVector result{};

						for (const auto& directory : directories)
						{
							result.push_back(new WAssetExplorerItem(directory));
						}

						for (const auto& file : files)
						{
							result.push_back(new WAssetExplorerItem(file.Path().string()));
						}

						return result;
					},
				}),
			}),
		},
	});
}
