#pragma once
#include "Asset.h"
#include "Path.h"
#include <concepts>
#include <unordered_map>
template <std::derived_from<AAsset> T>
class SAssetManager final
{
public:
	static void Cleanup()
	{
		for (auto& [path, asset] : assets_)
		{
			delete asset;
		}
		assets_.clear();
	}

	static T* Get(const UPath& path)
	{
		const auto it{ assets_.find(path) };
		return it != assets_.end() ? it->second : nullptr;
	}

	template <typename... Args>
		requires std::constructible_from<T, Args...>
	static T* Create(Args&&... args)
	{
		T* asset{ new T{ std::forward<Args>(args)... } };
		assets_[asset->GetPath()] = asset;
		return asset;
	}

private:
	inline static std::unordered_map<UPath, T*> assets_;
};