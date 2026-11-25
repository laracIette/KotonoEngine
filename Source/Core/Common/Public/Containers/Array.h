#pragma once

#include "type_traits.h"
#include "types.h"

#include <utility>

template <typename T, ::size N>
    requires(N > 0)
class UArray final
{
private:
    using ValueType = T;
    using IndexType = ::size;

public:
    using value_type = ValueType;

public:
    constexpr UArray() noexcept = default;
    constexpr UArray(UArray&&) noexcept = default;
    constexpr UArray(UArray const&) noexcept = default;

    template <typename... Args>
        requires (sizeof...(Args) == N) && VAllConvertibleTo<ValueType, Args...>
    constexpr UArray(Args&&... args) noexcept((std::is_nothrow_convertible_v<Args, ValueType> && ...))
        : data_{ static_cast<ValueType>(std::forward<Args>(args))... }
    {}

    constexpr auto operator=(UArray&&) noexcept -> UArray& = default;
    constexpr auto operator=(UArray const&) noexcept -> UArray& = default;

    static constexpr auto Size() noexcept -> IndexType
    {
        return N;
    }

    constexpr void Fill(ValueType const& value) noexcept(std::is_nothrow_copy_constructible_v<ValueType>)
    {
        for (IndexType i{ 0 }; i < N; ++i)
        {
            data_[i] = value;
        }
    }

    constexpr auto Data(this auto&& self) noexcept
    {
        return self.data_;
    }

    constexpr decltype(auto) operator[](this auto&& self, IndexType index) noexcept
    {
        return std::forward_like<decltype(self)>(self.data_[index]);
    }

    [[nodiscard]] constexpr auto begin(this auto&& self) noexcept
    {
        return self.data_ + 0;
    }

    [[nodiscard]] constexpr auto end(this auto&& self) noexcept
    {
        return self.data_ + N;
    }

private:
    ValueType data_[N];
};

template <typename First, typename... Rest>
    requires VAllConvertibleTo<First, Rest...>
UArray(First&&, Rest&&...) -> UArray<std::decay_t<First>, 1 + sizeof...(Rest)>;
