#include "AssetExplorerDirectory.h"

WAssetExplorerDirectory::WAssetExplorerDirectory(UPath path, OpenedCallback onOpened)
	: Base(std::move(path), std::move(onOpened))
{
}

#include "AssetExplorerDirectory.generated.inl"
