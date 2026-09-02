#pragma once
#include "generated/AssetExplorerDirectory.generated.h"
#include "AssetExplorerItem.h"
class WAssetExplorerDirectory : public WAssetExplorerItem
{
	GENERATED_WASSETEXPLORERDIRECTORY()

public:
	WAssetExplorerDirectory(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, const OnClickedFunc& onDoubleClicked);
};