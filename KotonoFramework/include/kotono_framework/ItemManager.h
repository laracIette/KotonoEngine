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
		const char* typeName = typeid(T).name();
		KT_DEBUG_LOG("cleaning up %s", typeName);

		for (auto& [path, item] : _items)
		{
			CleanupItem(item);
			delete item;
		}

		KT_DEBUG_LOG("cleaned up %s", typeName);
	}

	T* Get(const std::filesystem::path& path)
	{
		if (!_items.contains(path))
		{
			_items[path] = CreateItem(path);
		}

		return _items[path];
	}

	virtual T* CreateItem(const std::filesystem::path& path) const = 0;
	virtual void CleanupItem(T* item) const = 0;

private:
	std::unordered_map<std::filesystem::path, T*> _items;
};
