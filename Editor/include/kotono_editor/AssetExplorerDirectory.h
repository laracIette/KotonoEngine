#pragma once
#include "AssetExplorerItem.h"
class WAssetExplorerDirectory : public WAssetExplorerItem
{
public:
	WAssetExplorerDirectory(const UPath& path, const OnClickedFunc& onClicked);
};