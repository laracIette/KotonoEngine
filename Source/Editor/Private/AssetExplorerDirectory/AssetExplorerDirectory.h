#pragma once
#include "AssetExplorerItem/AssetExplorerItem.h"

#include "AssetExplorerDirectory.generated.h"

class WAssetExplorerDirectory : public WAssetExplorerItem
{
	GENERATED()

public:
	WAssetExplorerDirectory(UPath path, OpenedCallback onOpened);
};