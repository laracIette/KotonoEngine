#pragma once
#include <functional>
#include <vector>
#include <span>
#include "Pool.h"
/// <summary>
/// Non-owning container wrapper with filtering
/// </summary>
template <std::input_iterator IteratorType>
class KtCollection final
{
private:
	using ValueType = typename std::iterator_traits<IteratorType>::value_type;
	using FilterFunction = std::function<bool(const ValueType&)>;

	struct Iterator final
	{
		Iterator(IteratorType current, IteratorType end, std::span<const FilterFunction> filters) :
			current_(current), 
			end_(end), 
			filters_(filters)
		{
			AdvanceToNextValid();
		}

		const ValueType& operator*() const
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

		bool PassesFilters(const ValueType& item) const
		{
			for (const FilterFunction& filter : filters_)
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
		std::span<const FilterFunction> filters_;
	};

public:
	KtCollection(const IteratorType& begin, const IteratorType& end)
		: begin_(begin), end_(end) {}

	void AddFilter(FilterFunction&& filter)
	{
		filters_.push_back(std::move(filter));
	}

	void AddFilter(const FilterFunction& filter)
	{
		filters_.push_back(filter);
	}

	const std::vector<ValueType> GetVector() const
	{
		std::vector<ValueType> result{};
		result.reserve(std::distance(begin_, end_));
		for (const ValueType& item : *this)
		{
			result.push_back(item);
		}
		return result;
	}

	const KtPool<ValueType> GetPool() const
	{
		KtPool<ValueType> result{};
		result.Reserve(std::distance(begin_, end_));
		for (const ValueType& item : *this)
		{
			result.Add(item);
		}
		return result;
	}

	ValueType GetFirst()
	{
		for (const ValueType& item : *this)
		{
			return item;
		}

		return ValueType();
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
	std::vector<FilterFunction> filters_;
};

