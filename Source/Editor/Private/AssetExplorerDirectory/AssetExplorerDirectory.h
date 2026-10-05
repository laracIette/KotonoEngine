#pragma once
#include "AssetExplorerDirectory.generated.h"
#include "AssetExplorerItem/AssetExplorerItem.h"

class WAssetExplorerDirectory : public WAssetExplorerItem
{
	GENERATED_WASSETEXPLORERDIRECTORY()

public:
	WAssetExplorerDirectory(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OpenedCallback const& onOpened);
};