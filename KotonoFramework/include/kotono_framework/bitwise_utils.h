#pragma once
#include <type_traits>

template <typename T>
    requires std::is_enum_v<T>
constexpr bool has_flag(T value, T flag) noexcept
{
    return (value & flag) == flag;
}