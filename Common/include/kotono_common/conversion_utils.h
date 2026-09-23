#pragma once
#include <charconv>
#include <format>
#include <ranges>
#include <string_view>

template<typename T>
static T from_string(std::string_view str, T defaultValue = T{})
{
    T result;
    auto [ptr, ec] { std::from_chars(str.data(), str.data() + str.size(), result) };
    return ec == std::errc{} ? result : defaultValue;
}

template <std::ranges::range R>
static std::string to_string(R&& range)
{
	return std::format("{0}", std::forward<R>(range));
}
