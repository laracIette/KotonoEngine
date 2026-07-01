#pragma once
#include "Asset.h"
#include "Path.h"
#include <unordered_map>
template <typename T>
class SAssetManager final
{
private:
	friend class GCore;

	using Asset = UAsset<T>;

public:
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

	static void Cleanup()
	{
		for (auto& [path, asset] : assets_)
		{
			delete asset.Get();
		}
		assets_.clear();
	}

private:
	inline static std::unordered_map<UPath, Asset> assets_;
};