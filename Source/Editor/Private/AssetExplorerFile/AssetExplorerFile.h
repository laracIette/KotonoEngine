#pragma once
#include "generated/AssetExplorerFile.generated.h"
#include "AssetExplorerItem/AssetExplorerItem.h"

class WAssetExplorerFile : public WAssetExplorerItem
{
	GENERATED_WASSETEXPLORERFILE()

public:
	WAssetExplorerFile(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path);
};