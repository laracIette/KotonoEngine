#pragma once
#include <unordered_map>
#include <filesystem>
#include "log.h"
template <typename T>
class KtItemManager
{
public:
	void Init()
	{
	}

	void Cleanup()
	{
		for (auto& [path, item] : items_)
		{
			CleanupItem(item);
			delete item;
		}
		items_.clear();
	}

	T* Get(const std::filesystem::path& path) const
	{
		const auto it = items_.find(path);
		if (it != items_.end())
		{
			return it->second;
		}
		return nullptr;
	}

	virtual T* Create(const std::filesystem::path& path) = 0;

protected:
	std::unordered_map<std::filesystem::path, T*> items_;

	virtual void CleanupItem(T* item) const = 0;
};
