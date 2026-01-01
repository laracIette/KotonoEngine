#pragma once
#include <vector>
#include <functional>
#include <concepts>
#include "types.h"

enum class KtPoolRemoveResult : char
{
	IndexOutOfRange,
	ItemNotFound,
	ItemRemoved,
	ItemSwappedAndRemoved
};

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
	KtPool() : data_() {}

	KtPool(IteratorType begin, IteratorType end) : data_(begin, end) {}

	KtPool(ConstIteratorType begin, ConstIteratorType end) : data_(begin, end) {}

	template <std::input_iterator CustomIteratorType>
	KtPool(CustomIteratorType begin, CustomIteratorType end) : data_(begin, end) {}

	template <typename T>
		requires std::constructible_from<ValueType, T&&>
	constexpr void Add(T&& value)
	{
		data_.push_back(std::forward<T>(value));
	}

	constexpr void Add(const ValueType& value)
	{
		data_.push_back(value);
	}

	// Remove the specified item with O(n) complexity
	constexpr KtPoolRemoveResult Remove(const ValueType& value)
	{
		const auto it{ std::find(data_.begin(), data_.end(), value) };
		if (it == data_.end())
		{
			return KtPoolRemoveResult::ItemNotFound;
		}

		const ::size index{ static_cast<::size>(std::distance(data_.begin(), it)) };
		return RemoveAt(index);
	}

	// Remove the item at the specified index with O(1) complexity
	constexpr KtPoolRemoveResult RemoveAt(const ::size index) noexcept
	{
		if (index >= data_.size())
		{
			return KtPoolRemoveResult::IndexOutOfRange;
		}

		auto result{ KtPoolRemoveResult::ItemRemoved };
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
		for (i64 i{ static_cast<i64>(data_.size()) - 1 }; i >= 0; --i)
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

	constexpr void reserve(const ::size size)
	{
		data_.reserve(size);
	}

	constexpr void resize(const ::size size)
	{
		data_.resize(size);
	}

	constexpr void Append(const KtPool& pool)
	{
		data_.insert(end(), pool.begin(), pool.end());
	}

	constexpr void Append(const std::initializer_list<ValueType>& values)
	{
		data_.insert(end(), values.begin(), values.end());
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

	constexpr size size() const noexcept
	{
		return data_.size();
	}

	// Returns -1 if size == 0
	constexpr i64 LastIndex() const noexcept
	{
		return static_cast<i64>(data_.size()) - 1;
	}

	constexpr bool empty() const noexcept
	{
		return data_.empty();
	}

	const ValueType& operator[](const ::size index) const noexcept
	{
		return data_[index];
	}

	ValueType& operator[](const ::size index) noexcept
	{
		return data_[index];
	}

private:
	VectorType data_;
};

