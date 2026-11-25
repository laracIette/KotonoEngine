#pragma once

#include "type_traits.h"
#include "types.h"

#include <ranges>
#include <vector>

template <typename T>
class UList final
{
private:
    using ValueType = T;
    using IndexType = ::size;
    using VectorType = std::vector<ValueType>;

public:
    using value_type = ValueType;

public:
    constexpr UList() noexcept = default;
    constexpr UList(UList&&) noexcept = default;
    constexpr UList(UList const&) = default;

    template <std::input_iterator It, std::sentinel_for<It> Sentinel>
    constexpr UList(It begin, Sentinel end)
        : data_(std::ranges::subrange(begin, end) | std::ranges::to<VectorType>())
    {}

    constexpr UList(std::initializer_list<ValueType> data)
        : UList(data.begin(), data.end())
    {}

    template <std::ranges::input_range R>
        requires (!std::derived_from<std::remove_cvref_t<R>, UList>)
    constexpr UList(R&& range)
        : UList(std::ranges::begin(range), std::ranges::end(range))
    {}

    constexpr auto operator=(UList&&) noexcept -> UList& = default;
    constexpr auto operator=(UList const&) -> UList& = default;

    constexpr void Reserve(IndexType capacity)
    {
        data_.reserve(capacity);
    }

    constexpr void Resize(IndexType size)
    {
        data_.resize(size);
    }

    constexpr void Resize(IndexType size, ValueType const& value)
    {
        data_.resize(size, value);
    }

    constexpr void Add(ValueType&& value)
    {
        data_.push_back(Move(value));
    }

    constexpr void Add(ValueType const& value)
    {
        data_.push_back(value);
    }

    constexpr void Append(std::initializer_list<ValueType> list)
    {
        data_.insert(data_.end(), list);
    }

    template <std::ranges::input_range R>
    constexpr void Append(R&& range)
    {
        data_.insert_range(data_.end(), Move(range));
    }

    constexpr void Pop() noexcept
    {
        data_.pop_back();
    }

    constexpr void Erase(VectorType::const_iterator first, VectorType::const_iterator last) noexcept
    {
        data_.erase(first, last);
    }

    constexpr void Clear() noexcept
    {
        data_.clear();
    }

    [[nodiscard]] constexpr auto Size() const noexcept -> IndexType
    {
        return data_.size();
    }

    [[nodiscard]] constexpr auto SizeBytes() const noexcept -> IndexType
    {
        return data_.size() * sizeof(ValueType);
    }

    [[nodiscard]] constexpr auto Capacity() const noexcept -> IndexType
    {
        return data_.capacity();
    }

    [[nodiscard]] constexpr auto CapacityBytes() const noexcept -> IndexType
    {
        return data_.capacity() * sizeof(ValueType);
    }

    [[nodiscard]] constexpr auto IsEmpty() const noexcept -> b8
    {
        return Size() == 0;
    }

    constexpr auto Data(this auto&& self) noexcept
    {
        return self.data_.data();
    }

    constexpr decltype(auto) First(this auto&& self) noexcept
    {
        return self.data_.front();
    }

    constexpr decltype(auto) Last(this auto&& self) noexcept
    {
        return self.data_.back();
    }

    constexpr decltype(auto) operator[](this auto&& self, IndexType index) noexcept
    {
        return self.data_[index];
    }

    constexpr auto begin(this auto&& self) noexcept
    {
        return self.data_.begin();
    }

    constexpr auto end(this auto&& self) noexcept
    {
        return self.data_.end();
    }

private:
    VectorType data_;
};

template <std::ranges::input_range R>
UList(R&&) -> UList<std::ranges::range_value_t<R>>;
