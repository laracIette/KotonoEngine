#pragma once
#include <charconv>
#include <concepts>
#include <format>
#include <ranges>
#include <string_view>

template<typename T>
inline T from_string(std::string_view str, T defaultValue = T{})
{
    T result;
    auto [ptr, ec] { std::from_chars(str.data(), str.data() + str.size(), result) };
    return ec == std::errc{} ? result : defaultValue;
}

template <std::ranges::input_range R>
inline std::string to_string(R&& range)
{
	return std::format("{0} items: {1}", std::ranges::size(range), std::forward<R>(range));
}

template <typename T>
	requires !std::is_floating_point_v<T>
inline std::string to_string(T&& v)
{
	return std::format("{0}", std::forward<T>(v));
}

template <std::floating_point T>
inline std::string to_string(T v)
{
	return std::format("{0:f}", v);
}
