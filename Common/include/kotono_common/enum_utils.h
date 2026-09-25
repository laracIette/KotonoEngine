#pragma once
#include <type_traits>

template <typename T>
concept ScopedEnum = std::is_scoped_enum_v<T>;

template <typename T, typename TEnum>
concept ConvertibleToUnderlying = std::is_same_v<T, TEnum> || std::convertible_to<T, std::underlying_type_t<TEnum>>;

template <ScopedEnum TEnum, ConvertibleToUnderlying<TEnum> TOther>
constexpr TEnum operator&(TEnum left, TOther right) noexcept
{
    const auto l{ static_cast<std::underlying_type_t<TEnum>>(left) };
    const auto r{ static_cast<std::underlying_type_t<TEnum>>(right) };
    return static_cast<TEnum>(l & r);
}

template <ScopedEnum TEnum, ConvertibleToUnderlying<TEnum> TOther>
constexpr TEnum operator|(TEnum left, TOther right) noexcept
{
    const auto l{ static_cast<std::underlying_type_t<TEnum>>(left) };
    const auto r{ static_cast<std::underlying_type_t<TEnum>>(right) };
    return static_cast<TEnum>(l | r);
}

template <ScopedEnum TEnum, ConvertibleToUnderlying<TEnum> TOther>
constexpr TEnum& operator&=(TEnum& left, TOther right) noexcept
{
    left = left & right;
    return left;
}

template <ScopedEnum TEnum, ConvertibleToUnderlying<TEnum> TOther>
constexpr TEnum& operator|=(TEnum& left, TOther right) noexcept
{
    left = left | right;
    return left;
}

template <ScopedEnum T>
constexpr T& operator++(T& value) noexcept
{
    using U = std::underlying_type_t<T>;
    value = static_cast<T>(static_cast<U>(value) + U{ 1 });
    return value;
}

template <ScopedEnum T>
constexpr T operator~(T value) noexcept
{
    using U = std::underlying_type_t<T>;
    return static_cast<T>(~static_cast<U>(value));
}

template <std::integral TInt, ScopedEnum TEnum>
constexpr TInt operator<<(TInt left, TEnum right) noexcept
{
    const auto r{ static_cast<std::underlying_type_t<TEnum>>(right) };
    return left << r;
}

template <ScopedEnum T>
constexpr bool has_flag(T value, T flag) noexcept
{
    return (value & flag) == flag;
}
