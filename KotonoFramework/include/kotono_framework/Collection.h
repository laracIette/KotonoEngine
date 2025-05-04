#pragma once
#include <span>
#include <functional>
#include <vector>
template<typename T>
class KtCollection
{
public:
	typedef std::function<bool(T)> Filter;

	KtCollection(const std::span<T> data) : data_(data) {}

	void AddFilter(const Filter& filter)
	{
		filters_.push_back(filter);
	}

	const std::vector<T> GetResult() const
	{
		std::vector<T> result;
		for (const auto& item : data_)
		{
			bool isItemValid = true;
			for (const auto& filter : filters_)
			{
				if (!filter(item))
				{
					isItemValid = false;
					break;
				}
			}
			if (isItemValid)
			{
				result.push_back(item);
			}
		}

		return result;
	}

private:
	std::span<T> data_;
	std::vector<Filter> filters_;
};

