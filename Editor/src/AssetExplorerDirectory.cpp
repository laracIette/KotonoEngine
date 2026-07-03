#include "AssetExplorerDirectory.h"

WAssetExplorerDirectory::WAssetExplorerDirectory(const UPath& path, const OnClickedFunc& onDoubleClicked)
	: Base(path, onDoubleClicked)
{
}

#include "generated/AssetExplorerDirectory.generated.inl"
