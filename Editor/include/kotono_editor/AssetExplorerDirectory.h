#pragma once
#include "generated/AssetExplorerDirectory.generated.h"
#include "AssetExplorerItem.h"
class WAssetExplorerDirectory : public WAssetExplorerItem
{
	GENERATED_WASSETEXPLORERDIRECTORY()

public:
	WAssetExplorerDirectory(const UPath& path, const OnClickedFunc& onDoubleClicked);
};