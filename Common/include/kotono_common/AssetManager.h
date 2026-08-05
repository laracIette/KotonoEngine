#pragma once
#include "Asset.h"
#include "Path.h"
#include <unordered_map>
template <typename T>
class SAssetManager final
{
public:
	using Asset = UAsset<T>;

public:
	static void Cleanup()
	{
		for (auto& [path, asset] : assets_)
		{
			delete asset.Get();
		}
		assets_.clear();
	}

	static Asset Get(const UPath& path)
	{
		const auto it{ assets_.find(path) };
		return it != assets_.end() ? it->second : Create(path);
	}

private:
	static Asset Create(const UPath& path)
	{
		Asset asset(path, new T{ path });
		assets_[path] = asset;
		return asset;
	}

private:
	inline static std::unordered_map<UPath, Asset> assets_;
};