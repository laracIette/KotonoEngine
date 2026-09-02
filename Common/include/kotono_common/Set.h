#pragma once
#include "Pool.h"
#include <concepts>
#include <ranges>
#include <unordered_map>
/// <summary>
/// An contiguous container for unique items with O(1) item lookup,
/// using a std::unordered_map for looking up item indices.
/// Respects insertion order but performs swap and pop when removing items.
/// </summary>
template <typename ValueType>
class USet final
{
private:
	using PoolType = UPool<ValueType>;
	using IndexType = PoolType::index_type;
	using IndicesMapType = std::unordered_map<ValueType, IndexType>;
	using IndicesMapIterator = IndicesMapType::iterator;
	using IndicesMapConstIterator = IndicesMapType::const_iterator;

public:
	using value_type = ValueType;

public:
	USet() = default;

	USet(USet const& set) 
		: values_(set.values_)
		, indices_(set.indices_)
	{
	}

	template <std::input_iterator It, std::sentinel_for<It> Sentinel>
	USet(It begin, Sentinel end)
		: values_(begin, end)
	{
		PopulateIndices();
	}

	USet(std::initializer_list<ValueType> data)
		: USet(data.begin(), data.end())
	{
	}

	template <std::ranges::input_range R>
		requires (!std::derived_from<std::remove_cvref_t<R>, USet>)
	USet(R&& range)
		: USet(std::ranges::begin(range), std::ranges::end(range))
	{
	}

	template <typename T>
		requires std::constructible_from<ValueType, T>
	USet(USet<T> const& set)
	{
		for (auto const& item : set)
		{
			values_.push_back(item);
		}
		PopulateIndices();
	}

	USet& operator=(USet const& set)
	{
		values_ = set.values_;
		indices_ = set.indices_;
		return *this;
	}

	template <typename T>
		requires std::constructible_from<ValueType, T&&>
	void Add(T&& value)
	{
		if (Contains(value))
		{
			return;
		}

		values_.Add(std::forward<T>(value));

		ValueType const& insertedValue{ values_.back() };
		indices_[insertedValue] = values_.LastIndex();
	}

	auto Find(this auto&& self, ValueType const& value)
	{
		return self.indices_.find(value);
	}

	void Remove(IndicesMapConstIterator it)
	{
		if (it == indices_.end())
		{
			return;
		}

		IndexType const index{ it->second };

		if (values_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
		{
			ValueType const& movedValue{ values_[index] };
			indices_[movedValue] = index;
		}

		indices_.erase(it);
	}

	void Remove(ValueType const& value)
	{
		Remove(Find(value));
	}

	bool Contains(ValueType const& value) const
	{
		return indices_.contains(value);
	}

	void Clear() noexcept
	{
		values_.Clear();
		indices_.clear();
	}

	constexpr i64 LastIndex() const noexcept
	{
		return values_.LastIndex();
	}

	constexpr b8 IsValidIndex(IndexType index) const noexcept
	{
		return values_.IsValidIndex(index);
	}

	constexpr b8 IsValidIndex(i64 index) const noexcept
	{
		return values_.IsValidIndex(index);
	}

	constexpr auto begin(this auto&& self) noexcept(noexcept(std::ranges::begin(self.values_)))
	{
		return std::ranges::begin(self.values_);
	}

	constexpr auto end(this auto&& self) noexcept(noexcept(std::ranges::end(self.values_)))
	{
		return std::ranges::end(self.values_);
	}

	constexpr auto back(this auto&& self) noexcept(noexcept(self.values_.back()))
	{
		return self.values_.back();
	}

	constexpr auto operator[](this auto&& self, IndexType index) noexcept(noexcept(self.values_[index]))
	{
		return self.values_[index];
	}

	void push_back(auto&& value)
	{
		Add(std::forward<decltype(value)>(value));
	}

	constexpr void reserve(IndexType size)
	{
		values_.reserve(size);
	}

	constexpr IndexType size() const noexcept
	{
		return values_.size();
	}

	constexpr b8 empty() const noexcept
	{
		return values_.empty();
	}

private:
	void PopulateIndices()
	{
		for (auto const& [index, value] : values_ | std::views::enumerate)
		{
			indices_[value] = static_cast<IndexType>(index);
		}
	}

private:
	PoolType values_;
	IndicesMapType indices_;
};

template <std::ranges::input_range R>
USet(R&&) -> USet<std::ranges::range_value_t<R>>;
