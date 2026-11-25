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
	USet(USet&&) noexcept = default;
	USet(USet const&) = default;

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
			values_.Add(item);
		}
		PopulateIndices();
	}

	auto operator=(USet&&) noexcept -> USet& = default;
	auto operator=(USet const&) -> USet& = default;

	template <typename T>
		requires std::constructible_from<ValueType, T&&>
	void Add(T&& value)
	{
		if (Contains(value))
		{
			return;
		}

		values_.Add(std::forward<T>(value));

		ValueType const& insertedValue{ values_.Last() };
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

	void Remove(ValueType const& value)
	{
		Remove(Find(value));
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
	
	void Replace(ValueType const& oldValue, ValueType const& newValue)
	{
		Replace(Find(oldValue), newValue);
	}

	void Clear() noexcept
	{
		values_.Clear();
		indices_.Clear();
	}

    constexpr void Reserve(IndexType size)
	{
	    values_.Reserve(size);
	}

    [[nodiscard]] constexpr auto Size() const noexcept -> IndexType
	{
	    return values_.Size();
	}

    [[nodiscard]] constexpr auto IsEmpty() const noexcept -> b8
	{
	    return values_.IsEmpty();
	}

	[[nodiscard]] constexpr auto LastIndex() const noexcept -> i64
	{
		return values_.LastIndex();
	}

	[[nodiscard]] constexpr auto IsValidIndex(IndexType index) const noexcept -> b8
	{
		return values_.IsValidIndex(index);
	}

	[[nodiscard]] constexpr auto IsValidIndex(i64 index) const noexcept -> b8
	{
		return values_.IsValidIndex(index);
	}

    [[nodiscard]] auto Contains(ValueType const& value) const -> b8
	{
	    return indices_.Contains(value);
	}

	[[nodiscard]] auto IsValidIterator(IndicesMapConstIterator it) const noexcept -> b8
	{
		return indices_.IsValidIterator(it);
	}
	
	[[nodiscard]] auto GetIndex(ValueType const& value) const -> IndexType
	{
		return indices_.At(value);
	}

	constexpr auto Data(this auto&& self) noexcept
	{
		return self.values_.Data();
	}

	constexpr decltype(auto) First(this auto&& self) noexcept
	{
		return self.values_.First();
	}

	constexpr decltype(auto) Last(this auto&& self) noexcept
	{
		return self.values_.Last();
	}

	constexpr decltype(auto) operator[](this auto&& self, IndexType index) noexcept(noexcept(self.values_[index]))
	{
		return self.values_[index];
	}

    constexpr auto begin(this auto&& self) noexcept(noexcept(self.values_.begin()))
	{
	    return self.values_.begin();
	}

    constexpr auto end(this auto&& self) noexcept(noexcept(self.values_.end()))
	{
	    return self.values_.end();
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
