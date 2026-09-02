#include "AssetExplorerDirectory.h"

WAssetExplorerDirectory::WAssetExplorerDirectory(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, const OnClickedFunc& onDoubleClicked)
	: Base(assetExplorer, path, onDoubleClicked)
{
}

#include "generated/AssetExplorerDirectory.generated.inl"
