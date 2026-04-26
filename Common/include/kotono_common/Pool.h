#pragma once
#include <vector>
#include <functional>
#include <concepts>
#include "types.h"

enum class KtPoolRemoveResult : u8
{
	IndexOutOfRange,
	ItemNotFound,
	ItemRemoved,
	ItemSwappedAndRemoved
};

/// <summary>
/// std::vector wrapper with O(1) item removal
/// </summary>
template <typename ValueType>
class UPool final
{
private:
	using VectorType = std::vector<ValueType>;
	using IteratorType = VectorType::iterator;
	using ConstIteratorType = VectorType::const_iterator;
	using ConditionFunction = std::function<bool(const ValueType&)>;
	using IndexType = ::size;

public:
	using value_type = ValueType;

public:
	UPool() : data_() {}

	UPool(IteratorType begin, IteratorType end) : data_(begin, end) {}

	UPool(ConstIteratorType begin, ConstIteratorType end) : data_(begin, end) {}

	template <std::input_iterator CustomIteratorType>
	UPool(CustomIteratorType begin, CustomIteratorType end) : data_(begin, end) {}

	UPool(std::initializer_list<ValueType> data) : data_(data) {}
	
	template <typename T>
		requires std::constructible_from<ValueType, T>
	UPool(const UPool<T>& pool)
	{
		for (auto& item : pool)
		{
			data_.push_back(item);
		}
	}

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
		const i64 index{ FindIndex(value) };
		if (index == -1)
		{
			return KtPoolRemoveResult::ItemNotFound;
		}

		return RemoveAt(index);
	}

	// Remove the item at the specified index with O(1) complexity
	constexpr KtPoolRemoveResult RemoveAt(const IndexType index) noexcept
	{
		if (!IsValidIndex(index))
		{
			return KtPoolRemoveResult::IndexOutOfRange;
		}

		auto result{ KtPoolRemoveResult::ItemRemoved };
		if (index != static_cast<IndexType>(LastIndex()))
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
		for (i64 i{ LastIndex() }; IsValidIndex(i); --i)
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

	constexpr ConstIteratorType Find(const ValueType& value) const
	{
		return std::find(data_.begin(), data_.end(), value);
	}

	constexpr i64 FindIndex(const ValueType& value) const
	{
		const auto it{ Find(value) };
		if (it == end())
		{
			return -1;
		}

		return static_cast<IndexType>(std::distance(data_.begin(), it));
	}

	constexpr void reserve(const IndexType size)
	{
		data_.reserve(size);
	}

	constexpr void resize(const IndexType size)
	{
		data_.resize(size);
	}

	constexpr void Append(const UPool& pool)
	{
		data_.insert(end(), pool.begin(), pool.end());
	}

	constexpr void Append(const std::initializer_list<ValueType>& values)
	{
		data_.insert(end(), values.begin(), values.end());
	}

	constexpr void Merge(UPool& pool)
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
		return static_cast<i64>(size()) - 1;
	}

	constexpr bool IsValidIndex(const IndexType index) const noexcept
	{
		return index >= 0 && index < size();
	}

	constexpr bool IsValidIndex(const i64 index) const noexcept
	{
		return index >= 0 && index < size();
	}

	constexpr bool empty() const noexcept
	{
		return data_.empty();
	}

	const ValueType& operator[](const IndexType index) const noexcept
	{
		return data_[index];
	}

	ValueType& operator[](const IndexType index) noexcept
	{
		return data_[index];
	}

	void push_back(const value_type& item)
	{
		data_.push_back(item);
	}
	void push_back(value_type&& item)
	{
		data_.push_back(std::move(item));
	}

private:
	VectorType data_;
};

