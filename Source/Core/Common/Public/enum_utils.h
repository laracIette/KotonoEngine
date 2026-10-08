#pragma once
#include <type_traits>
#include <utility>

template <typename T>
concept ScopedEnum = std::is_scoped_enum_v<T>;

template <typename T, typename TEnum>
concept ConvertibleToUnderlying = std::is_same_v<T, TEnum> || std::convertible_to<T, std::underlying_type_t<TEnum>>;

template <typename From, typename To>
concept ConvertibleTo = std::is_same_v<From, To> || std::is_convertible_v<
	std::conditional_t<std::is_scoped_enum_v<From>, 
		std::underlying_type_t<From>, 
		From
	>, 
	To
>;

template <ScopedEnum TEnum, ConvertibleToUnderlying<TEnum> TOther>
constexpr TEnum operator&(TEnum left, TOther right) noexcept
{
    const auto l{ std::to_underlying(left) };
    const auto r{ static_cast<std::underlying_type_t<TEnum>>(right) };
    return static_cast<TEnum>(l & r);
}

template <ScopedEnum TEnum, ConvertibleToUnderlying<TEnum> TOther>
constexpr TEnum operator|(TEnum left, TOther right) noexcept
{
    const auto l{ std::to_underlying(left) };
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
	value = static_cast<T>(std::to_underlying(value) + 1);
	return value;
}

template <ScopedEnum T>
constexpr T operator++(T& value, i32) noexcept
{
	T const temp{ value };
	++value;
	return temp;
}

template <ScopedEnum T>
constexpr T operator~(T value) noexcept
{
    return static_cast<T>(~std::to_underlying(value));
}

template <std::integral TInt, ScopedEnum TEnum>
constexpr TInt operator<<(TInt left, TEnum right) noexcept
{
    const auto r{ std::to_underlying(right) };
    return left << r;
}

template <ScopedEnum T>
constexpr bool has_flag(T value, T flag) noexcept
{
    return (value & flag) == flag;
}
