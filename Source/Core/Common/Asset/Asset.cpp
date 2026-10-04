#include "Asset.h"

AAsset::AAsset(UPath const& path)
	: path_{ path }
{
}

UPath const& AAsset::GetPath() const
{
	return path_;
}
