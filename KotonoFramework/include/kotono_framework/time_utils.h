#pragma once
#include <type_traits>
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

constexpr Numeric auto ms_to_ns(const Numeric auto v)
{
    return v * static_cast<decltype(v)>(1000000);
}

constexpr Numeric auto s_to_ms(const Numeric auto v)
{
    return v * static_cast<decltype(v)>(1000);
}