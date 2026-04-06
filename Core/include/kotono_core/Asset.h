#pragma once
#include <kotono_common/Path.h>

template <typename T>
class UAsset;

template <typename T>
class UAssetManager final
{
private:
	using Asset = UAsset<T>;

public:
	static Asset Get(const UPath& path)
	{

	}

private:
	inline static std::unordered_map<UPath, Asset> assets_;
};

template <typename T>
class UAsset final
{
private:
	friend struct std::hash<UAsset>;

public:
	UAsset(const UPath& path) : pointer_(nullptr) {}

private:
	T* pointer_;
};

template <typename T>
struct std::hash<UAsset<T>>
{
	::size operator()(const UAsset<T>& asset) const
	{
		return asset.
	}
};