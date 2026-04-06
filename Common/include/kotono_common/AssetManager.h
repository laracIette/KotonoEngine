#pragma once
#include "Path.h"
#include <unordered_map>

template <typename T>
class UAssetManager final
{
public:
	static T* Get(const UPath& path)
	{
		const auto it{ assets_.find(path) };
		return it != assets_.end() ? it->second : Create(path);
	}

	static T* Create(const UPath& path)
	{
		T* asset{ new T(path) };
		assets_[path] = asset;
		return asset;
	}

	static void Cleanup()
	{
		for (auto& [path, asset] : assets_)
		{
			delete asset;
		}
		assets_.clear();
	}

private:
	inline static std::unordered_map<UPath, T*> assets_;
};