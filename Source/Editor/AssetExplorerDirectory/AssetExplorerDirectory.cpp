#include "AssetExplorerDirectory.h"

WAssetExplorerDirectory::WAssetExplorerDirectory(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OpenedCallback const& onOpened)
	: Base(assetExplorer, path, onOpened)
{
}

#include "generated/AssetExplorerDirectory.generated.inl"
