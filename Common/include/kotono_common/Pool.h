#pragma once
#include "types.h"
#include <concepts>
#include <functional>
#include <ranges>
#include <vector>
enum class EPoolRemoveResult : u8
{
	IndexOutOfRange,
	ItemNotFound,
	ItemRemoved,
	ItemSwappedAndRemoved,
};
/// <summary>
/// std::vector wrapper with O(1) item removal.
/// </summary>
template <typename ValueType>
class UPool final
{
private:
	using VectorType = std::vector<ValueType>;
	using ConditionFunction = std::function<b8(ValueType const&)>;
	using IndexType = ::size;

public:
	using value_type = ValueType;
	using index_type = IndexType;

public:
	constexpr UPool() noexcept = default;

	template <std::input_iterator It, std::sentinel_for<It> Sentinel>
	constexpr UPool(It begin, Sentinel end)
		: data_(std::ranges::subrange(begin, end) | std::ranges::to<VectorType>())
	{}

	constexpr UPool(std::initializer_list<ValueType> data) 
		: UPool(data.begin(), data.end())
	{}

	template <std::ranges::input_range R>
		requires (!std::derived_from<std::remove_cvref_t<R>, UPool>)
	constexpr UPool(R&& range)
		: UPool(std::ranges::begin(range), std::ranges::end(range))
	{}
	
	template <typename T>
		requires std::constructible_from<ValueType, T>
	constexpr UPool(UPool<T> const& pool)
	{
		for (auto const& item : pool)
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

	// Remove the item at the specified index with O(1) complexity
	constexpr EPoolRemoveResult RemoveAt(IndexType index) noexcept
	{
		if (!IsValidIndex(index))
		{
			return EPoolRemoveResult::IndexOutOfRange;
		}

		auto result{ EPoolRemoveResult::ItemRemoved };
		if (index != static_cast<IndexType>(LastIndex()))
		{
			// Only swap if not last
			data_[index] = std::move(data_.back()); 
			result = EPoolRemoveResult::ItemSwappedAndRemoved;
		}
		data_.pop_back();
		return result;
	}

	constexpr void RemoveIf(ConditionFunction const& condition) noexcept
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

	constexpr void reserve(IndexType size)
	{
		data_.reserve(size);
	}

	constexpr void resize(IndexType size)
	{
		data_.resize(size);
	}

	constexpr IndexType size() const noexcept
	{
		return data_.size();
	}

	constexpr b8 empty() const noexcept
	{
		return data_.empty();
	}

	constexpr void Append(UPool const& pool)
	{
		data_.insert(end(), pool.begin(), pool.end());
	}

	constexpr void Append(std::initializer_list<ValueType> const& values)
	{
		data_.insert(end(), values.begin(), values.end());
	}

	constexpr void Merge(UPool& pool)
	{
		data_.insert(end(), std::make_move_iterator(pool.begin()), std::make_move_iterator(pool.end()));
		pool.Clear();
	}

	constexpr auto begin(this auto&& self) noexcept(noexcept(std::ranges::begin(self.data_)))
	{
		return std::ranges::begin(self.data_);
	}

	constexpr auto end(this auto&& self) noexcept(noexcept(std::ranges::end(self.data_)))
	{
		return std::ranges::end(self.data_);
	}

	constexpr auto back(this auto&& self) noexcept(noexcept(self.data_.back()))
	{
		return self.data_.back();
	}

	constexpr auto operator[](this auto&& self, IndexType index) noexcept(noexcept(self.data_[index]))
	{
		return self.data_[index];
	}

	// Returns -1 if size == 0
	constexpr i64 LastIndex() const noexcept
	{
		return static_cast<i64>(data_.size()) - 1;
	}

	constexpr b8 IsValidIndex(IndexType index) const noexcept
	{
		return index >= 0 && index < data_.size();
	}

	constexpr b8 IsValidIndex(i64 index) const noexcept
	{
		return index >= 0 && index < data_.size();
	}

	constexpr void push_back(ValueType const& item)
	{
		data_.push_back(item);
	}

	constexpr void push_back(ValueType&& item)
	{
		data_.push_back(std::move(item));
	}

private:
	VectorType data_;
};

template <std::ranges::input_range R>
UPool(R&&) -> UPool<std::ranges::range_value_t<R>>;
