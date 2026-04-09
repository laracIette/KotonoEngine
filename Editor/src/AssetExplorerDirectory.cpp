#include "AssetExplorerDirectory.h"

WAssetExplorerDirectory::WAssetExplorerDirectory(const UPath& path, const OnClickedFunc& onDoubleClicked)
	: WAssetExplorerItem(path, onDoubleClicked)
{
}

#include "generated/AssetExplorerDirectory.generated.inl"
