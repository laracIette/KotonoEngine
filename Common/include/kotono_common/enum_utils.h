#pragma once
#include <type_traits>
template <typename T>
    requires std::is_enum_v<T>
inline constexpr bool has_flag(T value, T flag) noexcept
{
    const auto v{ static_cast<std::underlying_type_t<T>>(value) };
    const auto f{ static_cast<std::underlying_type_t<T>>(flag) };
    return (v & f) == f;
}

template <typename T>
    requires std::is_enum_v<T>
inline constexpr T bitwise_and(T left, T right) noexcept
{
    const auto l{ static_cast<std::underlying_type_t<T>>(left) };
    const auto r{ static_cast<std::underlying_type_t<T>>(right) };
    return static_cast<T>(l & r);
}

template <typename T>
    requires std::is_enum_v<T>
inline constexpr T bitwise_or(T left, T right) noexcept
{
    const auto l{ static_cast<std::underlying_type_t<T>>(left) };
    const auto r{ static_cast<std::underlying_type_t<T>>(right) };
    return static_cast<T>(l | r);
}

template <typename T>
    requires std::is_enum_v<T>
inline constexpr T &operator++(T &value) noexcept
{
    using value_type = std::underlying_type_t<T>;
    value = static_cast<T>(static_cast<value_type>(value) + value_type{ 1 });
    return value;
}