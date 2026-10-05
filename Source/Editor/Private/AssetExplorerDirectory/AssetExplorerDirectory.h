#pragma once
#include "AssetExplorerItem/AssetExplorerItem.h"

#include "AssetExplorerDirectory.generated.h"

class WAssetExplorerDirectory : public WAssetExplorerItem
{
	GENERATED()

public:
	WAssetExplorerDirectory(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OpenedCallback const& onOpened);
};