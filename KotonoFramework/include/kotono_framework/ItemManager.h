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
		for (auto& [path, item] : _items)
		{
			CleanupItem(item);
			delete item;
		}
		_items.clear();
	}

	T* Get(const std::filesystem::path& path)
	{
		if (!_items.contains(path))
		{
			return nullptr;
		}

		return _items[path];
	}

	virtual T* Create(const std::filesystem::path& path) = 0;

protected:
	std::unordered_map<std::filesystem::path, T*> _items;

	virtual void CleanupItem(T* item) const = 0;
};
