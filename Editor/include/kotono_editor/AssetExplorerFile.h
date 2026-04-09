#pragma once
#include "generated/AssetExplorerFile.generated.h"
#include "AssetExplorerItem.h"
class WAssetExplorerFile : public WAssetExplorerItem
{
	GENERATED_WASSETEXPLORERFILE()

public:
	WAssetExplorerFile(const UPath& path);
};