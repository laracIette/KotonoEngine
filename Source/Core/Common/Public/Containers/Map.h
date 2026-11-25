#pragma once

#include "type_traits.h"
#include "types.h"

#include <unordered_map>

/// <summary>
/// A wrapper for std::unordered_map with utility functions
/// </summary>
template <typename KeyType, typename ValueType>
class UMap final
{
private:
	using UnorderedMapType = std::unordered_map<KeyType, ValueType>;
	using UnorderedMapValueType = UnorderedMapType::value_type;
	using UnorderedMapIterator = UnorderedMapType::iterator;
	using UnorderedMapConstIterator = UnorderedMapType::const_iterator;
	
public:
	using value_type = UnorderedMapValueType;
	using iterator = UnorderedMapIterator;
	using const_iterator = UnorderedMapConstIterator;
	
public:
    constexpr UMap() = default;
    constexpr UMap(UMap&&) noexcept = default;
    constexpr UMap(UMap const&) = default;
	
	UMap(std::initializer_list<UnorderedMapValueType> list)
		: data_(list.begin(), list.end())
	{
	}
	
	auto operator=(UMap&&) noexcept -> UMap& = default;
	auto operator=(UMap const&) -> UMap& = default;
	
	decltype(auto) At(this auto&& self, KeyType const& key)
	{
		return self.data_.at(key);
	}

	auto operator[](KeyType&& key) -> ValueType&
	{
		return data_[Move(key)];
	}

	auto operator[](KeyType const& key) -> ValueType&
	{
		return data_[key];
	}
	
	void Clear() noexcept
	{
		data_.clear();
	}
	
	template <typename... TArgs>
		requires std::constructible_from<ValueType, TArgs...>
	auto TryEmplace(KeyType&& key, TArgs&&... value)
	{
		return data_.try_emplace(std::move(key), std::forward<TArgs>(value)...);
	}
	
	template <typename... TArgs>
		requires std::constructible_from<ValueType, TArgs...>
	auto TryEmplace(KeyType const& key, TArgs&&... value)
	{
		return data_.try_emplace(key, std::forward<TArgs>(value)...);
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

    auto begin(this auto&& self) noexcept(noexcept(self.data_.begin()))
	{
	    return self.data_.begin();
	}

    auto end(this auto&& self) noexcept(noexcept(self.data_.end()))
	{
	    return self.data_.end();
	}
	
private:
	UnorderedMapType data_;
};
