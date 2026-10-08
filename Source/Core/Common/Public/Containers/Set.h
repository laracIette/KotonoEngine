#pragma once
#include "Map.h"
#include "Pool.h"
#include <concepts>
#include <ranges>

/// <summary>
/// A contiguous container for unique items with O(1) item lookup,
/// using a UMap for looking up item indices.
/// Respects insertion order but performs swap and pop when removing items.
/// </summary>
template <typename ValueType>
class USet final
{
private:
	using PoolType = UPool<ValueType>;
	using IndexType = PoolType::index_type;
	using IndicesMapType = UMap<ValueType, IndexType>;
	using IndicesMapIterator = IndicesMapType::iterator;
	using IndicesMapConstIterator = IndicesMapType::const_iterator;

public:
	using value_type = ValueType;

public:
	USet() = default;

	USet(USet&& set) noexcept
		: values_(std::move(set.values_))
		, indices_(std::move(set.indices_))
	{
	}

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

	auto operator=(USet&& set) noexcept -> USet&
	{
		values_ = std::move(set.values_);
		indices_ = std::move(set.indices_);
		return *this;
	}

	auto operator=(USet const& set) -> USet&
	{
		if (this == &set)
		{
			return *this;
		}
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
		indices_.TryEmplace(insertedValue, values_.LastIndex());
	}

	auto Find(this auto&& self, ValueType const& value)
	{
		return self.indices_.Find(value);
	}

	void Remove(IndicesMapConstIterator it)
	{
		if (!IsValidIterator(it))
		{
			return;
		}

		IndexType const index{ it->second };

		if (values_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
		{
			ValueType const& movedValue{ values_[index] };
			indices_.InsertOrAssign(movedValue, index);
		}

		indices_.Erase(it);
	}

	void Replace(IndicesMapConstIterator it, ValueType const& value)
	{
		if (!IsValidIterator(it))
		{
			Add(value);
			return;
		}

		IndexType const index{ it->second };

		indices_.Erase(it);

		values_[index] = value;
		indices_.InsertOrAssign(value, index);
	}

	void Remove(ValueType const& value)
	{
		Remove(Find(value));
	}
	
	void Replace(ValueType const& oldValue, ValueType const& newValue)
	{
		Replace(Find(oldValue), newValue);
	}

	bool Contains(ValueType const& value) const
	{
		return indices_.Contains(value);
	}

	void Clear() noexcept
	{
		values_.Clear();
		indices_.Clear();
	}

	constexpr auto LastIndex() const noexcept -> i64
	{
		return values_.LastIndex();
	}

	constexpr auto IsValidIndex(IndexType index) const noexcept -> b8
	{
		return values_.IsValidIndex(index);
	}

	constexpr auto IsValidIndex(i64 index) const noexcept -> b8
	{
		return values_.IsValidIndex(index);
	}

	constexpr auto IsValidIterator(IndicesMapConstIterator it) const noexcept -> b8
	{
		return indices_.IsValidIterator(it);
	}

	constexpr auto begin(this auto&& self) noexcept(noexcept(std::ranges::begin(self.values_)))
	{
		return std::ranges::begin(self.values_);
	}

	constexpr auto end(this auto&& self) noexcept(noexcept(std::ranges::end(self.values_)))
	{
		return std::ranges::end(self.values_);
	}

	constexpr decltype(auto) back(this auto&& self) noexcept(noexcept(self.values_.back()))
	{
		return self.values_.back();
	}

	constexpr decltype(auto) operator[](this auto&& self, IndexType index) noexcept(noexcept(self.values_[index]))
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

	constexpr auto size() const noexcept -> IndexType
	{
		return values_.size();
	}

	constexpr auto empty() const noexcept -> b8
	{
		return values_.empty();
	}

private:
	void PopulateIndices()
	{
		for (auto const& [index, value] : values_ | std::views::enumerate)
		{
			indices_.TryEmplace(value, static_cast<IndexType>(index));
		}
	}

private:
	PoolType values_;
	IndicesMapType indices_;
};

template <std::ranges::input_range R>
USet(R&&) -> USet<std::ranges::range_value_t<R>>;
