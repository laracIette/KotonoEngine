#pragma once
#include "AssetExplorerItem/AssetExplorerItem.h"

#include "AssetExplorerFile.generated.h"

class WAssetExplorerFile : public WAssetExplorerItem
{
	GENERATED()

public:
	WAssetExplorerFile(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path);
};