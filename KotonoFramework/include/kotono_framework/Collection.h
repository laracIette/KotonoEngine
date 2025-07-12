#pragma once
#include <functional>
#include <vector>
#include <span>
#include "Pool.h"
template <typename IteratorType>
class KtCollection final
{
public:
	using T = typename std::iterator_traits<IteratorType>::value_type;
	using Filter = std::function<bool(const T&)>;

	struct Iterator final
	{
		Iterator(IteratorType current, IteratorType end, std::span<const Filter> filters)
			: current_(current), end_(end), filters_(filters)
		{
			AdvanceToNextValid();
		}

		const T& operator*() const
		{
			return *current_;
		}

		Iterator& operator++()
		{
			++current_;
			AdvanceToNextValid();
			return *this;
		}

		bool operator!=(const Iterator& other) const
		{
			return current_ != other.current_;
		}

	private:
		void AdvanceToNextValid()
		{
			while (current_ != end_ && !PassesFilters(*current_))
			{
				++current_;
			}
		}

		bool PassesFilters(const T& item) const
		{
			for (const Filter& filter : filters_)
			{
				if (!filter(item))
				{
					return false;
				}
			}
			return true;
		}

		IteratorType current_;
		IteratorType end_;
		std::span<const Filter> filters_;
	};

	KtCollection(const IteratorType& begin, const IteratorType& end)
		: begin_(begin), end_(end) {}

	void AddFilter(Filter&& filter)
	{
		filters_.push_back(std::move(filter));
	}

	void AddFilter(const Filter& filter)
	{
		filters_.push_back(filter);
	}

	const std::vector<T> GetVector() const
	{
		std::vector<T> result;
		for (const T& item : *this)
		{
			result.push_back(item);
		}
		return result;
	}

	const KtPool<T> GetPool() const
	{
		KtPool<T> result;
		for (const T& item : *this)
		{
			result.Add(item);
		}
		return result;
	}

	T GetFirst()
	{
		for (const T& item : *this)
		{
			return item;
		}

		return T();
	}

	Iterator begin() const 
	{ 
		return Iterator(begin_, end_, filters_);
	}

	Iterator end() const 
	{
		return Iterator(end_, end_, filters_);
	}

private:
	IteratorType begin_;
	IteratorType end_;
	std::vector<Filter> filters_;
};

