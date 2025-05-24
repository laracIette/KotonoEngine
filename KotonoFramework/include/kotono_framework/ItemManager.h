#pragma once
#include <unordered_map>
#include <filesystem>
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

	T* Get(const std::filesystem::path& path)
	{
		const auto it = items_.find(path);
		if (it != items_.end())
		{
			return it->second;
		}
		return Create(path);
	}

	const bool Exists(const std::filesystem::path& path) const
	{
		return items_.contains(path);
	}

protected:
	virtual T* Create(const std::filesystem::path& path) = 0;
	virtual void CleanupItem(T* item) const = 0;

	void Add(const std::filesystem::path& path, T* item)
	{
		items_[path] = item;
	}

private:
	std::unordered_map<std::filesystem::path, T*> items_;
};
