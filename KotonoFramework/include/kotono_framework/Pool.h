#pragma once
#include <vector>
#include <functional>
#include "PoolRemoveResult.h"
/// <summary>
/// std::vector wrapper with fast item removal
/// </summary>
template <typename ValueType>
class KtPool final
{
private:
	using VectorType = std::vector<ValueType>;
	using IteratorType = VectorType::iterator;
	using ConstIteratorType = VectorType::const_iterator;
	using ConditionFunction = std::function<bool(const ValueType&)>;

public:
	template <typename T>
		requires std::constructible_from<ValueType, T&&>
	constexpr void Add(T&& value)
	{
		data_.push_back(std::forward<T>(value));
	}

	// Removes the specified item, returns true if the item was swapped before removal
	// This is way slower than RemoveAt(size_t) because it searches for the item's index first
	constexpr const KtPoolRemoveResult Remove(const ValueType& value)
	{
		const auto it{ std::find(data_.begin(), data_.end(), value) };
		if (it == data_.end())
		{
			return KtPoolRemoveResult::ItemNotFound;
		}

		const size_t index{ static_cast<size_t>(std::distance(data_.begin(), it)) };
		return RemoveAt(index);
	}

	// Removes the item at the specified index, returns true if the item was swapped before removal
	constexpr const KtPoolRemoveResult RemoveAt(const size_t index) noexcept
	{
		if (index >= data_.size())
		{
			return KtPoolRemoveResult::IndexOutOfRange;
		}

		KtPoolRemoveResult result{ KtPoolRemoveResult::ItemRemoved };
		if (index != data_.size() - 1)
		{
			// Only swap if not last
			data_[index] = std::move(data_.back()); 
			result = KtPoolRemoveResult::ItemSwappedAndRemoved;
		}
		data_.pop_back();
		return result;
	}

	constexpr void RemoveIf(const ConditionFunction& condition) noexcept
	{
		// Have to check backwards to avoid skipping condition checks
		for (int64_t i{ static_cast<int64_t>(data_.size()) - 1 }; i >= 0; --i)
		{
			if (condition(data_[i]))
			{
				RemoveAt(i);
			}
		}
	}

	constexpr void Clear() noexcept
	{
		data_.clear();
	}

	constexpr void Reserve(const size_t size)
	{
		data_.reserve(size);
	}

	constexpr void Append(const KtPool& pool)
	{
		data_.insert(end(), pool.begin(), pool.end());
	}

	constexpr void Merge(KtPool& pool)
	{
		data_.insert(end(), std::make_move_iterator(pool.begin()), std::make_move_iterator(pool.end()));
		pool.Clear();
	}

	constexpr IteratorType begin() noexcept
	{
		return data_.begin(); 
	}

	constexpr IteratorType end() noexcept
	{ 
		return data_.end(); 
	}

	constexpr ConstIteratorType begin() const noexcept
	{
		return data_.begin();
	}

	constexpr ConstIteratorType end() const noexcept
	{ 
		return data_.end(); 
	}

	constexpr const size_t Size() const noexcept
	{
		return data_.size();
	}

	// Returns size_t max if size == 0
	constexpr const size_t LastIndex() const noexcept
	{
		return data_.size() - 1;
	}

	constexpr const bool Empty() const noexcept
	{
		return data_.empty();
	}

	const ValueType& operator[](const size_t index) const noexcept
	{
		return data_[index];
	}

	ValueType& operator[](const size_t index) noexcept
	{
		return data_[index];
	}

private:
	VectorType data_;
};

