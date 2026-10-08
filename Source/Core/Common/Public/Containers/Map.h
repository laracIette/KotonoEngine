#pragma once
#include <ranges>
#include <type_traits>
#include <unordered_map>

/// <summary>
/// A wrapper for std::unordered_map with utility functions
/// </summary>
template <typename KeyType, typename ValueType>
class UMap final
{
private:
	using UnorderedMapType = std::unordered_map<KeyType, ValueType>;
	using UnorderedMapIterator = UnorderedMapType::iterator;
	using UnorderedMapConstIterator = UnorderedMapType::const_iterator;
	
public:
	using iterator = UnorderedMapIterator;
	using const_iterator = UnorderedMapConstIterator;
	
public:
	UMap() = default;
	
	UMap(UMap&& map) noexcept
		: data_(std::move(map.data_))
	{
	}
	
	UMap(UMap const& map)
		: data_(map.data_)
	{
	}	
	
	UMap& operator=(UMap&& map) noexcept
	{
		data_ = std::move(map.data_);
		return *this;
	}

	UMap& operator=(UMap const& map)
	{
		if (this == &map)
		{
			return *this;
		}
		data_ = map.data_;
		return *this;
	}
	
	auto begin(this auto&& self) noexcept(noexcept(std::ranges::begin(self.data_)))
	{
		return std::ranges::begin(self.data_);
	}
	
	auto end(this auto&& self) noexcept(noexcept(std::ranges::end(self.data_)))
	{
		return std::ranges::end(self.data_);
	}

	decltype(auto) operator[](this auto&& self, KeyType const& key) noexcept(noexcept(self.data_[key]))
	{
		return self.data_[key];
	}
	
	void Clear() noexcept
	{
		data_.clear();
	}
	
	template <typename... TVals>
		requires std::constructible_from<ValueType, TVals...>
	auto TryEmplace(KeyType&& key, TVals&&... value)
	{
		return data_.try_emplace(std::move(key), std::forward<TVals>(value)...);
	}
	
	template <typename... TVals>
		requires std::constructible_from<ValueType, TVals...>
	auto TryEmplace(KeyType const& key, TVals&&... value)
	{
		return data_.try_emplace(key, std::forward<TVals>(value)...);
	}
	
	auto Find(this auto&& self, KeyType const& key)
	{
		return self.data_.find(key);
	}
	
	auto IsValidIterator(UnorderedMapConstIterator it) const noexcept -> b8
	{
		return it != data_.end();
	}
	
	auto Contains(KeyType const& key) const -> b8
	{
		return data_.contains(key);
	}
	
	void Remove(KeyType const& key)
	{
		Remove(Find(key));
	}
	
	auto Erase(UnorderedMapConstIterator it) -> UnorderedMapIterator
	{
		return data_.erase(it);
	}
	
	/// Safe Erase, returns if invalid iterator
	void Remove(UnorderedMapConstIterator it)
	{
		if (!IsValidIterator(it))
		{
			return;
		}

		Erase(it);
	}
	
	template <typename TVal>
		requires std::constructible_from<ValueType, TVal> && std::assignable_from<ValueType&, TVal>
	auto InsertOrAssign(KeyType&& key, TVal&& value)
	{
		return data_.insert_or_assign(std::move(key), std::forward<TVal>(value));
	}
	
	template <typename TVal>
		requires std::constructible_from<ValueType, TVal> && std::assignable_from<ValueType&, TVal>
	auto InsertOrAssign(KeyType const& key, TVal&& value)
	{
		return data_.insert_or_assign(key, std::forward<TVal>(value));
	}
	
	/// Returns a copy if found, else defaultValue
	auto FindOrDefault(KeyType const& key, ValueType const& defaultValue = {}) -> ValueType
	{
		auto const it{ Find(key) };
		return IsValidIterator(it) ? it->second : defaultValue;
	}
	
private:
	UnorderedMapType data_;
};
